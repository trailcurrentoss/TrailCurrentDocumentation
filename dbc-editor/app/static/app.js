// ============================================================
// State
// ============================================================
let state = {
    messages: [],
    nodes: [],
    filename: '',
    dirty: false,
};
let modalCallback = null;

// ============================================================
// API Client
// ============================================================
async function api(url, options = {}) {
    const resp = await fetch(url, {
        headers: { 'Content-Type': 'application/json', ...options.headers },
        ...options,
    });
    const data = await resp.json();
    if (!resp.ok) throw new Error(data.error || 'Request failed');
    return data;
}

async function fetchDatabase() {
    const data = await api('/api/database');
    state.messages = data.messages;
    state.nodes = data.nodes;
    state.filename = data.filename;
    state.dirty = false;
    updateUI();
}

// ============================================================
// Toast Notifications
// ============================================================
function showToast(message, type = 'info') {
    const container = document.getElementById('toast-container');
    const toast = document.createElement('div');
    toast.className = `toast toast-${type}`;
    toast.textContent = message;
    container.appendChild(toast);
    setTimeout(() => toast.remove(), 4000);
}

// ============================================================
// UI Updates
// ============================================================
function updateUI() {
    document.getElementById('filename').textContent = state.filename;
    const dirtyEl = document.getElementById('dirty-indicator');
    dirtyEl.style.display = state.dirty ? 'inline' : 'none';
    renderMessages();
    renderNodes();
}

function markDirty() {
    state.dirty = true;
    document.getElementById('dirty-indicator').style.display = 'inline';
}

// ============================================================
// Tab Navigation
// ============================================================
function switchTab(tabName) {
    document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
    document.querySelectorAll('.tab-content').forEach(t => t.classList.remove('active'));
    document.querySelector(`.tab[data-tab="${tabName}"]`).classList.add('active');
    document.getElementById(`tab-${tabName}`).classList.add('active');
}

// ============================================================
// Message Rendering
// ============================================================
function formatHexId(id) {
    return '0x' + id.toString(16).toUpperCase().padStart(3, '0');
}

function renderMessages() {
    const container = document.getElementById('messages-list');
    const search = (document.getElementById('message-search').value || '').toLowerCase();

    const filtered = state.messages.filter(m => {
        if (!search) return true;
        const hex = formatHexId(m.frame_id).toLowerCase();
        return m.name.toLowerCase().includes(search)
            || hex.includes(search)
            || String(m.frame_id).includes(search)
            || (m.senders[0] || '').toLowerCase().includes(search);
    });

    if (filtered.length === 0) {
        container.innerHTML = '<div class="empty-state"><p>No messages found</p></div>';
        return;
    }

    container.innerHTML = filtered.map(msg => renderMessageCard(msg)).join('');
}

function renderMessageCard(msg) {
    const hex = formatHexId(msg.frame_id);
    const sender = msg.senders[0] || 'Unknown';
    const sigCount = msg.signals.length;
    const commentHtml = msg.comment
        ? `<div class="message-comment">${escapeHtml(msg.comment)}</div>` : '';

    const signalsHtml = msg.signals.length > 0 ? `
        <table class="signals-table">
            <thead>
                <tr>
                    <th>Signal</th>
                    <th>Bit</th>
                    <th>Len</th>
                    <th>Order</th>
                    <th>Factor</th>
                    <th>Offset</th>
                    <th>Range</th>
                    <th>Unit</th>
                    <th>Description</th>
                    <th></th>
                </tr>
            </thead>
            <tbody>
                ${msg.signals.map(s => renderSignalRow(msg.frame_id, s)).join('')}
            </tbody>
        </table>
    ` : '<p style="color:var(--text-secondary);font-size:0.85rem;">No signals defined</p>';

    return `
    <div class="message-card" id="msg-${msg.frame_id}">
        <div class="message-header" onclick="toggleMessage(${msg.frame_id})">
            <span class="expand-icon">&#9654;</span>
            <span class="message-id">${hex}</span>
            <span class="message-name">${escapeHtml(msg.name)}</span>
            <div class="message-meta">
                <span>${msg.length} bytes</span>
                <span>${sigCount} signal${sigCount !== 1 ? 's' : ''}</span>
                <span>${escapeHtml(sender)}</span>
            </div>
            <div class="message-actions">
                <button class="btn-icon" onclick="event.stopPropagation(); showMessageModal(${msg.frame_id})" title="Edit message">&#9998;</button>
                <button class="btn-icon danger" onclick="event.stopPropagation(); deleteMessage(${msg.frame_id})" title="Delete message">&#128465;</button>
            </div>
        </div>
        <div class="message-body">
            ${commentHtml}
            <div class="signals-section">
                <h4>Signals</h4>
                ${signalsHtml}
                <div class="add-signal-row">
                    <button class="btn btn-success btn-sm" onclick="showSignalModal(${msg.frame_id})">+ Add Signal</button>
                </div>
            </div>
        </div>
    </div>`;
}

function renderSignalRow(frameId, sig) {
    const order = sig.byte_order === 'big_endian' ? 'BE' : 'LE';
    const range = `${sig.minimum}..${sig.maximum}`;
    const commentText = sig.comment || '';
    const truncComment = commentText.length > 60 ? commentText.substring(0, 60) + '...' : commentText;

    let choicesHtml = '';
    if (sig.choices && Object.keys(sig.choices).length > 0) {
        const chips = Object.entries(sig.choices)
            .map(([k, v]) => `<span>${k}=${escapeHtml(v)}</span>`).join('');
        choicesHtml = `<div class="signal-choices">${chips}</div>`;
    }

    return `
    <tr>
        <td><span class="signal-name">${escapeHtml(sig.name)}</span>${choicesHtml}</td>
        <td>${sig.start_bit}</td>
        <td>${sig.length}</td>
        <td>${order}</td>
        <td>${sig.factor}</td>
        <td>${sig.offset}</td>
        <td>${range}</td>
        <td>${escapeHtml(sig.unit)}</td>
        <td class="signal-comment-cell" title="${escapeHtml(commentText)}">${escapeHtml(truncComment)}</td>
        <td style="white-space:nowrap;">
            <button class="btn-icon" onclick="showSignalModal(${frameId}, '${escapeAttr(sig.name)}')" title="Edit">&#9998;</button>
            <button class="btn-icon danger" onclick="deleteSignal(${frameId}, '${escapeAttr(sig.name)}')" title="Delete">&#128465;</button>
        </td>
    </tr>`;
}

function toggleMessage(frameId) {
    const card = document.getElementById(`msg-${frameId}`);
    card.classList.toggle('expanded');
}

function filterMessages() {
    renderMessages();
}

// ============================================================
// Node Rendering
// ============================================================
function renderNodes() {
    const container = document.getElementById('nodes-list');
    if (state.nodes.length === 0) {
        container.innerHTML = '<div class="empty-state"><p>No nodes defined</p></div>';
        return;
    }
    container.innerHTML = state.nodes.map(node => `
        <div class="node-card">
            <div class="node-header">
                <span class="node-name">${escapeHtml(node.name)}</span>
                <div>
                    <button class="btn-icon" onclick="showNodeModal('${escapeAttr(node.name)}')" title="Edit">&#9998;</button>
                    <button class="btn-icon danger" onclick="deleteNode('${escapeAttr(node.name)}')" title="Delete">&#128465;</button>
                </div>
            </div>
            ${node.comment ? `<div class="node-comment">${escapeHtml(node.comment)}</div>` : ''}
        </div>
    `).join('');
}

// ============================================================
// Modal System
// ============================================================
function openModal(title, bodyHtml, callback) {
    document.getElementById('modal-title').textContent = title;
    document.getElementById('modal-body').innerHTML = bodyHtml;
    document.getElementById('modal-overlay').style.display = 'flex';
    modalCallback = callback;
    // Focus first input
    setTimeout(() => {
        const first = document.querySelector('.modal-body input, .modal-body select, .modal-body textarea');
        if (first) first.focus();
    }, 100);
}

function closeModal() {
    document.getElementById('modal-overlay').style.display = 'none';
    modalCallback = null;
}

function closeModalOnOverlay(event) {
    if (event.target === document.getElementById('modal-overlay')) {
        closeModal();
    }
}

function submitModal() {
    if (modalCallback) modalCallback();
}

// Handle Enter key in modal
document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') closeModal();
});

// ============================================================
// Message Modal
// ============================================================
function showMessageModal(frameId = null) {
    const isEdit = frameId !== null;
    const msg = isEdit ? state.messages.find(m => m.frame_id === frameId) : null;
    const title = isEdit ? `Edit Message: ${msg.name}` : 'Add New Message';

    const nodeOptions = state.nodes.map(n =>
        `<option value="${escapeAttr(n.name)}" ${msg && msg.senders[0] === n.name ? 'selected' : ''}>${escapeHtml(n.name)}</option>`
    ).join('');

    const html = `
        <div class="form-row">
            <div class="form-group">
                <label>Message Name</label>
                <input type="text" id="msg-name" value="${msg ? escapeAttr(msg.name) : ''}" placeholder="e.g. GpsDateTime">
            </div>
            <div class="form-group">
                <label>Frame ID (decimal)</label>
                <input type="number" id="msg-frame-id" value="${msg ? msg.frame_id : ''}" min="0" max="2047" placeholder="e.g. 6">
                <div class="hint" id="msg-hex-hint">${msg ? formatHexId(msg.frame_id) : ''}</div>
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label>Data Length (bytes)</label>
                <input type="number" id="msg-length" value="${msg ? msg.length : 8}" min="0" max="8">
            </div>
            <div class="form-group">
                <label>Sender</label>
                <select id="msg-sender">
                    <option value="">-- Select Node --</option>
                    ${nodeOptions}
                </select>
            </div>
        </div>
        <div class="form-group">
            <div class="checkbox-group">
                <input type="checkbox" id="msg-extended" ${msg && msg.is_extended_frame ? 'checked' : ''}>
                <label for="msg-extended">Extended Frame (29-bit ID)</label>
            </div>
        </div>
        <div class="form-group">
            <label>Comment</label>
            <textarea id="msg-comment" rows="4">${msg ? escapeHtml(msg.comment) : ''}</textarea>
        </div>
    `;

    openModal(title, html, async () => {
        const data = {
            name: document.getElementById('msg-name').value.trim(),
            frame_id: parseInt(document.getElementById('msg-frame-id').value),
            length: parseInt(document.getElementById('msg-length').value),
            sender: document.getElementById('msg-sender').value,
            is_extended_frame: document.getElementById('msg-extended').checked,
            comment: document.getElementById('msg-comment').value,
        };

        if (!data.name) { showToast('Message name is required', 'error'); return; }
        if (isNaN(data.frame_id)) { showToast('Frame ID is required', 'error'); return; }

        try {
            if (isEdit) {
                await api(`/api/messages/${frameId}`, { method: 'PUT', body: JSON.stringify(data) });
                showToast('Message updated');
            } else {
                await api('/api/messages', { method: 'POST', body: JSON.stringify(data) });
                showToast('Message added');
            }
            markDirty();
            await fetchDatabase();
            closeModal();
        } catch (e) {
            showToast(e.message, 'error');
        }
    });

    // Live hex preview
    document.getElementById('msg-frame-id').addEventListener('input', (e) => {
        const val = parseInt(e.target.value);
        document.getElementById('msg-hex-hint').textContent = isNaN(val) ? '' : formatHexId(val);
    });
}

async function deleteMessage(frameId) {
    const msg = state.messages.find(m => m.frame_id === frameId);
    if (!confirm(`Delete message "${msg.name}" (${formatHexId(frameId)})? This will also remove all its signals.`)) return;
    try {
        await api(`/api/messages/${frameId}`, { method: 'DELETE' });
        showToast('Message deleted');
        markDirty();
        await fetchDatabase();
    } catch (e) {
        showToast(e.message, 'error');
    }
}

// ============================================================
// Signal Modal
// ============================================================
function showSignalModal(frameId, signalName = null) {
    const msg = state.messages.find(m => m.frame_id === frameId);
    const isEdit = signalName !== null;
    const sig = isEdit ? msg.signals.find(s => s.name === signalName) : null;
    const title = isEdit ? `Edit Signal: ${sig.name}` : `Add Signal to ${msg.name}`;

    const receiversHtml = state.nodes.map(n => {
        const checked = sig && sig.receivers && sig.receivers.includes(n.name) ? 'checked' : '';
        return `<label class="receiver-chip"><input type="checkbox" value="${escapeAttr(n.name)}" ${checked}> ${escapeHtml(n.name)}</label>`;
    }).join('');

    let choicesRows = '';
    if (sig && sig.choices && Object.keys(sig.choices).length > 0) {
        choicesRows = Object.entries(sig.choices).map(([k, v]) =>
            `<div class="choices-row">
                <input type="number" class="choice-value" value="${k}" placeholder="Value">
                <input type="text" class="choice-desc" value="${escapeAttr(v)}" placeholder="Description">
                <button class="btn btn-danger btn-sm" onclick="this.parentElement.remove()">-</button>
            </div>`
        ).join('');
    }

    const html = `
        <div class="form-row">
            <div class="form-group">
                <label>Signal Name</label>
                <input type="text" id="sig-name" value="${sig ? escapeAttr(sig.name) : ''}" placeholder="e.g. Year">
            </div>
            <div class="form-group">
                <label>Unit</label>
                <input type="text" id="sig-unit" value="${sig ? escapeAttr(sig.unit) : ''}" placeholder="e.g. degC, V, A">
            </div>
        </div>
        <div class="form-row-3">
            <div class="form-group">
                <label>Start Bit</label>
                <input type="number" id="sig-start" value="${sig ? sig.start_bit : ''}" min="0" max="63">
                <div class="hint">MSB position for big-endian</div>
            </div>
            <div class="form-group">
                <label>Bit Length</label>
                <input type="number" id="sig-length" value="${sig ? sig.length : 8}" min="1" max="64">
            </div>
            <div class="form-group">
                <label>Byte Order</label>
                <select id="sig-byte-order">
                    <option value="big_endian" ${sig && sig.byte_order === 'big_endian' ? 'selected' : ''}>Big Endian (Motorola)</option>
                    <option value="little_endian" ${sig && sig.byte_order === 'little_endian' ? 'selected' : ''}>Little Endian (Intel)</option>
                </select>
            </div>
        </div>
        <div class="form-row-3">
            <div class="form-group">
                <label>Factor (scale)</label>
                <input type="number" id="sig-factor" value="${sig ? sig.factor : 1}" step="any">
                <div class="hint">physical = raw * factor + offset</div>
            </div>
            <div class="form-group">
                <label>Offset</label>
                <input type="number" id="sig-offset" value="${sig ? sig.offset : 0}" step="any">
            </div>
            <div class="form-group">
                <div class="checkbox-group" style="margin-top:20px;">
                    <input type="checkbox" id="sig-signed" ${sig && sig.is_signed ? 'checked' : ''}>
                    <label for="sig-signed">Signed</label>
                </div>
            </div>
        </div>
        <div class="form-row">
            <div class="form-group">
                <label>Minimum</label>
                <input type="number" id="sig-min" value="${sig ? sig.minimum : 0}" step="any">
            </div>
            <div class="form-group">
                <label>Maximum</label>
                <input type="number" id="sig-max" value="${sig ? sig.maximum : 0}" step="any">
            </div>
        </div>
        <div class="form-group">
            <label>Receivers</label>
            <div class="receivers-group" id="sig-receivers">
                ${receiversHtml}
            </div>
        </div>
        <div class="form-group">
            <label>Comment</label>
            <textarea id="sig-comment" rows="3">${sig ? escapeHtml(sig.comment) : ''}</textarea>
        </div>
        <div class="form-group">
            <label>Value Descriptions (optional)</label>
            <div class="hint" style="margin-bottom:8px;">Define named values for enum-like signals (e.g., 0 = "Off", 3 = "Bulk_Charging")</div>
            <div id="choices-container">
                ${choicesRows}
            </div>
            <button class="btn btn-secondary btn-sm" onclick="addChoiceRow()">+ Add Value</button>
        </div>
    `;

    openModal(title, html, async () => {
        // Collect receivers
        const receiverCheckboxes = document.querySelectorAll('#sig-receivers input[type="checkbox"]:checked');
        const receivers = Array.from(receiverCheckboxes).map(cb => cb.value);

        // Collect choices
        const choiceRows = document.querySelectorAll('#choices-container .choices-row');
        const choices = {};
        choiceRows.forEach(row => {
            const val = row.querySelector('.choice-value').value;
            const desc = row.querySelector('.choice-desc').value.trim();
            if (val !== '' && desc) choices[val] = desc;
        });

        const data = {
            name: document.getElementById('sig-name').value.trim(),
            start_bit: parseInt(document.getElementById('sig-start').value),
            length: parseInt(document.getElementById('sig-length').value),
            byte_order: document.getElementById('sig-byte-order').value,
            is_signed: document.getElementById('sig-signed').checked,
            factor: parseFloat(document.getElementById('sig-factor').value),
            offset: parseFloat(document.getElementById('sig-offset').value),
            minimum: parseFloat(document.getElementById('sig-min').value),
            maximum: parseFloat(document.getElementById('sig-max').value),
            unit: document.getElementById('sig-unit').value.trim(),
            comment: document.getElementById('sig-comment').value,
            receivers: receivers,
            choices: Object.keys(choices).length > 0 ? choices : null,
        };

        if (!data.name) { showToast('Signal name is required', 'error'); return; }
        if (isNaN(data.start_bit)) { showToast('Start bit is required', 'error'); return; }

        try {
            if (isEdit) {
                await api(`/api/messages/${frameId}/signals/${encodeURIComponent(signalName)}`, {
                    method: 'PUT', body: JSON.stringify(data)
                });
                showToast('Signal updated');
            } else {
                await api(`/api/messages/${frameId}/signals`, {
                    method: 'POST', body: JSON.stringify(data)
                });
                showToast('Signal added');
            }
            markDirty();
            await fetchDatabase();
            closeModal();
            // Re-expand the message card
            setTimeout(() => {
                const card = document.getElementById(`msg-${frameId}`);
                if (card) card.classList.add('expanded');
            }, 50);
        } catch (e) {
            showToast(e.message, 'error');
        }
    });
}

function addChoiceRow() {
    const container = document.getElementById('choices-container');
    const row = document.createElement('div');
    row.className = 'choices-row';
    row.innerHTML = `
        <input type="number" class="choice-value" placeholder="Value">
        <input type="text" class="choice-desc" placeholder="Description">
        <button class="btn btn-danger btn-sm" onclick="this.parentElement.remove()">-</button>
    `;
    container.appendChild(row);
}

async function deleteSignal(frameId, signalName) {
    if (!confirm(`Delete signal "${signalName}"?`)) return;
    try {
        await api(`/api/messages/${frameId}/signals/${encodeURIComponent(signalName)}`, { method: 'DELETE' });
        showToast('Signal deleted');
        markDirty();
        await fetchDatabase();
        // Re-expand
        setTimeout(() => {
            const card = document.getElementById(`msg-${frameId}`);
            if (card) card.classList.add('expanded');
        }, 50);
    } catch (e) {
        showToast(e.message, 'error');
    }
}

// ============================================================
// Node Modal
// ============================================================
function showNodeModal(nodeName = null) {
    const isEdit = nodeName !== null;
    const node = isEdit ? state.nodes.find(n => n.name === nodeName) : null;
    const title = isEdit ? `Edit Node: ${node.name}` : 'Add New Node';

    const html = `
        <div class="form-group">
            <label>Node Name</label>
            <input type="text" id="node-name" value="${node ? escapeAttr(node.name) : ''}" placeholder="e.g. GpsModule" ${isEdit ? 'readonly' : ''}>
            ${isEdit ? '<div class="hint">Node name cannot be changed (referenced by messages and signals)</div>' : ''}
        </div>
        <div class="form-group">
            <label>Comment / Description</label>
            <textarea id="node-comment" rows="4">${node ? escapeHtml(node.comment) : ''}</textarea>
        </div>
    `;

    openModal(title, html, async () => {
        const data = {
            name: document.getElementById('node-name').value.trim(),
            comment: document.getElementById('node-comment').value,
        };

        if (!data.name) { showToast('Node name is required', 'error'); return; }

        try {
            if (isEdit) {
                await api(`/api/nodes/${encodeURIComponent(nodeName)}`, {
                    method: 'PUT', body: JSON.stringify(data)
                });
                showToast('Node updated');
            } else {
                await api('/api/nodes', { method: 'POST', body: JSON.stringify(data) });
                showToast('Node added');
            }
            markDirty();
            await fetchDatabase();
            closeModal();
        } catch (e) {
            showToast(e.message, 'error');
        }
    });
}

async function deleteNode(name) {
    if (!confirm(`Delete node "${name}"? This will not remove it from existing message senders/receivers.`)) return;
    try {
        await api(`/api/nodes/${encodeURIComponent(name)}`, { method: 'DELETE' });
        showToast('Node deleted');
        markDirty();
        await fetchDatabase();
    } catch (e) {
        showToast(e.message, 'error');
    }
}

// ============================================================
// File Operations
// ============================================================
async function handleSave() {
    try {
        const result = await api('/api/save', { method: 'POST' });
        state.dirty = false;
        document.getElementById('dirty-indicator').style.display = 'none';
        showToast(result.message, 'success');
    } catch (e) {
        showToast('Save failed: ' + e.message, 'error');
    }
}

function handleDownload() {
    window.location.href = '/api/download';
}

async function handleReload() {
    if (state.dirty && !confirm('You have unsaved changes. Reload from disk?')) return;
    try {
        await api('/api/reload', { method: 'POST' });
        await fetchDatabase();
        showToast('Reloaded from disk', 'success');
    } catch (e) {
        showToast('Reload failed: ' + e.message, 'error');
    }
}

async function handleUpload(event) {
    event.preventDefault();
    const fileInput = document.getElementById('upload-input');
    if (!fileInput.files.length) return;

    const formData = new FormData();
    formData.append('file', fileInput.files[0]);

    try {
        const resp = await fetch('/api/upload', { method: 'POST', body: formData });
        const data = await resp.json();
        if (!resp.ok) throw new Error(data.error);
        showToast(data.message, 'success');
        markDirty();
        await fetchDatabase();
        switchTab('messages');
    } catch (e) {
        showToast('Upload failed: ' + e.message, 'error');
    }
}

// ============================================================
// Utilities
// ============================================================
function escapeHtml(str) {
    if (!str) return '';
    const div = document.createElement('div');
    div.textContent = str;
    return div.innerHTML;
}

function escapeAttr(str) {
    if (!str) return '';
    return str.replace(/&/g, '&amp;').replace(/"/g, '&quot;').replace(/'/g, '&#39;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

// ============================================================
// Initialize
// ============================================================
window.addEventListener('DOMContentLoaded', () => {
    fetchDatabase().catch(e => {
        showToast('Failed to load database: ' + e.message, 'error');
    });
});

// Warn before leaving with unsaved changes
window.addEventListener('beforeunload', (e) => {
    if (state.dirty) {
        e.preventDefault();
        e.returnValue = '';
    }
});
