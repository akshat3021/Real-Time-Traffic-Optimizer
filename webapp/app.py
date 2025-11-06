from flask import Flask, render_template, request
import subprocess
import os

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/find_route', methods=['POST'])
def find_route():
    data = request.get_json()
    start_node = data['start']
    end_node = data['end']

    try:
        if os.name == 'nt': 
            executable_path = 'engine\\optimizer.exe'
        else: 
            executable_path = 'engine/optimizer'

        command = [executable_path, start_node, end_node]
        
        process = subprocess.run(
            command,
            capture_output=True,
            text=True,
            check=True
        )
        return process.stdout
        
    except FileNotFoundError:
        return "{\"error\":\"Optimizer executable not found. Please compile the C code.\"}", 500
    except subprocess.CalledProcessError as e:
        return e.output, 500
    except Exception as e:
        return f"{{\"error\":\"An unknown server error occurred: {str(e)}\"}}", 500

if __name__ == '__main__':
    app.run(debug=True)