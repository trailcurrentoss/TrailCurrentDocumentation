import os
import shutil
import traceback
from datetime import datetime
from flask import Flask, jsonify, request, render_template, send_file
import cantools
from cantools.database.can import Message, Signal

app = Flask(__name__)

DBC_DIR = os.environ.get('DBC_DIR', '/data')
DBC_FILENAME = os.environ.get('DBC_FILENAME', 'TrailCurrent.dbc')

db = None


def get_dbc_path():
    return os.path.join(DBC_DIR, DBC_FILENAME)


def load_db():
    global db
    path = get_dbc_path()
    if os.path.exists(path):
        db = cantools.database.load_file(path)
    else:
        db = cantools.database.Database()


def get_node_names():
    if db is None:
        return []
    try:
        nodes = db.nodes
        if nodes and len(nodes) > 0:
            first = next(iter(nodes))
            if isinstance(first, str):
                return list(nodes)
            return [n.name for n in nodes]
    except Exception:
        pass
    return []


def get_node_comment(name):
    try:
        if hasattr(db, 'dbc') and db.dbc is not None:
            if hasattr(db.dbc, 'nodes'):
                for node in db.dbc.nodes:
                    if hasattr(node, 'name') and node.name == name:
                        return getattr(node, 'comment', '') or ''
    except Exception:
        pass
    return ''


def serialize_signal(sig):
    choices = {}
    if sig.choices:
        choices = {int(k): v for k, v in sig.choices.items()}
    return {
        'name': sig.name,
        'start_bit': sig.start,
        'length': sig.length,
        'byte_order': sig.byte_order,
        'is_signed': sig.is_signed,
        'factor': sig.scale,
        'offset': sig.offset,
        'minimum': sig.minimum if sig.minimum is not None else 0,
        'maximum': sig.maximum if sig.maximum is not None else 0,
        'unit': sig.unit or '',
        'comment': sig.comment or '',
        'receivers': list(sig.receivers) if sig.receivers else [],
        'choices': choices,
    }


def serialize_message(msg):
    return {
        'frame_id': msg.frame_id,
        'name': msg.name,
        'length': msg.length,
        'senders': list(msg.senders) if msg.senders else [],
        'comment': msg.comment or '',
        'signals': sorted(
            [serialize_signal(s) for s in msg.signals],
            key=lambda x: x['start_bit']
        ),
        'is_extended_frame': msg.is_extended_frame,
    }


# --- Routes ---

@app.route('/')
def index():
    return render_template('index.html')


@app.route('/api/database')
def get_database():
    node_names = get_node_names()
    messages = sorted(
        [serialize_message(m) for m in db.messages],
        key=lambda x: x['frame_id']
    )
    nodes = [{'name': n, 'comment': get_node_comment(n)} for n in node_names]
    return jsonify({
        'messages': messages,
        'nodes': nodes,
        'filename': DBC_FILENAME,
    })


# --- Message CRUD ---

@app.route('/api/messages', methods=['POST'])
def add_message():
    try:
        data = request.json
        frame_id = int(data['frame_id'])
        # Check for duplicate
        for m in db.messages:
            if m.frame_id == frame_id:
                return jsonify({'error': f'Message with ID {frame_id} already exists'}), 400

        msg = Message(
            frame_id=frame_id,
            name=data['name'],
            length=int(data.get('length', 8)),
            signals=[],
            comment=data.get('comment', ''),
            senders=[data['sender']] if data.get('sender') else [],
            is_extended_frame=data.get('is_extended_frame', False),
        )
        db._messages.append(msg)
        db.refresh()
        return jsonify(serialize_message(msg)), 201
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


@app.route('/api/messages/<int:frame_id>', methods=['PUT'])
def update_message(frame_id):
    try:
        msg = db.get_message_by_frame_id(frame_id)
        data = request.json

        new_frame_id = int(data.get('frame_id', frame_id))
        if new_frame_id != frame_id:
            for m in db.messages:
                if m.frame_id == new_frame_id:
                    return jsonify({'error': f'Message with ID {new_frame_id} already exists'}), 400

        msg._frame_id = new_frame_id
        msg._name = data.get('name', msg.name)
        msg._length = int(data.get('length', msg.length))
        msg._comment = data.get('comment', msg.comment)
        msg._senders = [data['sender']] if data.get('sender') else msg.senders
        msg._is_extended_frame = data.get('is_extended_frame', msg.is_extended_frame)

        if new_frame_id != frame_id:
            db.refresh()

        return jsonify(serialize_message(msg))
    except KeyError:
        return jsonify({'error': f'Message with ID {frame_id} not found'}), 404
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


@app.route('/api/messages/<int:frame_id>', methods=['DELETE'])
def delete_message(frame_id):
    try:
        msg = db.get_message_by_frame_id(frame_id)
        db._messages.remove(msg)
        db.refresh()
        return jsonify({'success': True})
    except KeyError:
        return jsonify({'error': f'Message with ID {frame_id} not found'}), 404
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


# --- Signal CRUD ---

@app.route('/api/messages/<int:frame_id>/signals', methods=['POST'])
def add_signal(frame_id):
    try:
        msg = db.get_message_by_frame_id(frame_id)
        data = request.json

        # Check for duplicate signal name
        for s in msg.signals:
            if s.name == data['name']:
                return jsonify({'error': f'Signal "{data["name"]}" already exists in this message'}), 400

        byte_order = data.get('byte_order', 'big_endian')
        choices = None
        if data.get('choices'):
            choices = {int(k): v for k, v in data['choices'].items()}

        sig = Signal(
            name=data['name'],
            start=int(data['start_bit']),
            length=int(data.get('length', 8)),
            byte_order=byte_order,
            is_signed=data.get('is_signed', False),
            scale=float(data.get('factor', 1)),
            offset=float(data.get('offset', 0)),
            minimum=float(data.get('minimum', 0)),
            maximum=float(data.get('maximum', 0)),
            unit=data.get('unit', ''),
            choices=choices,
            comment=data.get('comment', ''),
            receivers=data.get('receivers', []),
        )
        msg._signals.append(sig)
        msg.refresh()
        return jsonify(serialize_signal(sig)), 201
    except KeyError:
        return jsonify({'error': f'Message with ID {frame_id} not found'}), 404
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


@app.route('/api/messages/<int:frame_id>/signals/<signal_name>', methods=['PUT'])
def update_signal(frame_id, signal_name):
    try:
        msg = db.get_message_by_frame_id(frame_id)
        sig = None
        for s in msg.signals:
            if s.name == signal_name:
                sig = s
                break
        if sig is None:
            return jsonify({'error': f'Signal "{signal_name}" not found'}), 404

        data = request.json

        # Check for name conflict if renaming
        new_name = data.get('name', sig.name)
        if new_name != sig.name:
            for s in msg.signals:
                if s.name == new_name:
                    return jsonify({'error': f'Signal "{new_name}" already exists'}), 400

        sig._name = new_name
        sig._start = int(data.get('start_bit', sig.start))
        sig._length = int(data.get('length', sig.length))
        sig._byte_order = data.get('byte_order', sig.byte_order)
        sig._is_signed = data.get('is_signed', sig.is_signed)
        sig._scale = float(data.get('factor', sig.scale))
        sig._offset = float(data.get('offset', sig.offset))
        sig._minimum = float(data.get('minimum', sig.minimum or 0))
        sig._maximum = float(data.get('maximum', sig.maximum or 0))
        sig._unit = data.get('unit', sig.unit)
        sig._comment = data.get('comment', sig.comment)
        sig._receivers = data.get('receivers', sig.receivers)

        if 'choices' in data:
            if data['choices']:
                sig._choices = {int(k): v for k, v in data['choices'].items()}
            else:
                sig._choices = None

        msg.refresh()
        return jsonify(serialize_signal(sig))
    except KeyError:
        return jsonify({'error': f'Message with ID {frame_id} not found'}), 404
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


@app.route('/api/messages/<int:frame_id>/signals/<signal_name>', methods=['DELETE'])
def delete_signal(frame_id, signal_name):
    try:
        msg = db.get_message_by_frame_id(frame_id)
        sig = None
        for s in msg.signals:
            if s.name == signal_name:
                sig = s
                break
        if sig is None:
            return jsonify({'error': f'Signal "{signal_name}" not found'}), 404

        msg._signals.remove(sig)
        msg.refresh()
        return jsonify({'success': True})
    except KeyError:
        return jsonify({'error': f'Message with ID {frame_id} not found'}), 404
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


# --- Node CRUD ---

@app.route('/api/nodes', methods=['POST'])
def add_node():
    try:
        data = request.json
        name = data['name']
        comment = data.get('comment', '')

        existing = get_node_names()
        if name in existing:
            return jsonify({'error': f'Node "{name}" already exists'}), 400

        # Add to internal nodes list
        if hasattr(db, '_nodes'):
            # cantools stores nodes as Node objects or strings
            if db._nodes and len(db._nodes) > 0:
                first = db._nodes[0]
                if isinstance(first, str):
                    db._nodes.append(name)
                else:
                    # Try to create a Node-like object
                    try:
                        from cantools.database.can.node import Node as CanNode
                        node = CanNode(name=name, comment=comment)
                        db._nodes.append(node)
                    except ImportError:
                        db._nodes.append(name)
            else:
                db._nodes.append(name)
        # Also update dbc-specific nodes if available
        if hasattr(db, 'dbc') and db.dbc is not None:
            if hasattr(db.dbc, 'nodes'):
                try:
                    from cantools.database.can.node import Node as CanNode
                    db.dbc.nodes.append(CanNode(name=name, comment=comment))
                except (ImportError, AttributeError):
                    pass

        return jsonify({'name': name, 'comment': comment}), 201
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


@app.route('/api/nodes/<name>', methods=['PUT'])
def update_node(name):
    try:
        data = request.json
        comment = data.get('comment', '')

        # Update comment in dbc-specific nodes
        if hasattr(db, 'dbc') and db.dbc is not None:
            if hasattr(db.dbc, 'nodes'):
                for node in db.dbc.nodes:
                    if hasattr(node, 'name') and node.name == name:
                        node._comment = comment
                        break

        return jsonify({'name': name, 'comment': comment})
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


@app.route('/api/nodes/<name>', methods=['DELETE'])
def delete_node(name):
    try:
        if hasattr(db, '_nodes'):
            db._nodes = [
                n for n in db._nodes
                if (n if isinstance(n, str) else getattr(n, 'name', n)) != name
            ]
        if hasattr(db, 'dbc') and db.dbc is not None:
            if hasattr(db.dbc, 'nodes'):
                db.dbc.nodes = [
                    n for n in db.dbc.nodes
                    if getattr(n, 'name', n) != name
                ]
        return jsonify({'success': True})
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 400


# --- File operations ---

@app.route('/api/save', methods=['POST'])
def save_file():
    try:
        path = get_dbc_path()
        # Create backup
        if os.path.exists(path):
            timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
            backup_dir = os.path.join(DBC_DIR, 'backups')
            os.makedirs(backup_dir, exist_ok=True)
            backup_path = os.path.join(backup_dir, f'{DBC_FILENAME}.{timestamp}.bak')
            shutil.copy2(path, backup_path)

        # Write new file
        dbc_string = db.as_dbc_string()
        with open(path, 'w') as f:
            f.write(dbc_string)

        return jsonify({'success': True, 'message': 'File saved successfully'})
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500


@app.route('/api/download')
def download_file():
    try:
        dbc_string = db.as_dbc_string()
        return dbc_string, 200, {
            'Content-Type': 'application/octet-stream',
            'Content-Disposition': f'attachment; filename="{DBC_FILENAME}"'
        }
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500


@app.route('/api/upload', methods=['POST'])
def upload_file():
    try:
        if 'file' not in request.files:
            return jsonify({'error': 'No file provided'}), 400

        file = request.files['file']
        if file.filename == '':
            return jsonify({'error': 'No file selected'}), 400

        content = file.read().decode('utf-8')
        # Validate by trying to parse
        test_db = cantools.database.load_string(content, 'dbc')

        # If valid, replace the current database
        global db
        db = test_db
        return jsonify({
            'success': True,
            'message': f'Loaded {len(db.messages)} messages from uploaded file'
        })
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': f'Invalid DBC file: {str(e)}'}), 400


@app.route('/api/reload', methods=['POST'])
def reload_file():
    try:
        load_db()
        return jsonify({'success': True, 'message': 'Database reloaded from disk'})
    except Exception as e:
        traceback.print_exc()
        return jsonify({'error': str(e)}), 500


if __name__ == '__main__':
    load_db()
    app.run(host='0.0.0.0', port=5000, debug=True)
