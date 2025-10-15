<<<<<<< HEAD
from flask import Flask, render_template, request, jsonify
import subprocess

app = Flask(__name__)

# This route serves the main HTML page
@app.route('/')
def index():
    return render_template('index.html')

# This new route will be called by our JavaScript to find a path
@app.route('/find_route', methods=['POST'])
def find_route():
    data = request.get_json()
    start_node = data['start']
    end_node = data['end']

    try:
        # We will use the final C engine executable name: 'optimizer'
        # On Windows, this will be 'engine\\optimizer.exe'
        command = ['engine\\optimizer', start_node, end_node]
        
        process = subprocess.run(
            command,
            capture_output=True,
            text=True,
            check=True
        )
        # The C program's output is a JSON string of coordinates
        # We return it as a JSON response to the front-end
        return process.stdout
        
    except FileNotFoundError:
        return jsonify({"error": "Optimizer executable not found. Please compile the C code."}), 500
    except subprocess.CalledProcessError as e:
        return jsonify({"error": f"C program failed: {e.stderr}"}), 500
    except Exception as e:
        return jsonify({"error": str(e)}), 500
=======
from flask import Flask, render_template, request
import subprocess 

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def index():
    result = None
    if request.method == 'POST':
        source = request.form.get('source')
        destination = request.form.get('destination')

        
        try:
            # 
            command = ['engine\\fake_optimizer', source, destination]
            
            process = subprocess.run(
                command,
                capture_output=True,
                text=True,
                check=True 
            )
            result = process.stdout 
        except FileNotFoundError:
            result = "Error: The 'fake_optimizer' program was not found. Have you compiled it?"
        except subprocess.CalledProcessError as e:
            result = f"The C program failed with an error: {e.stderr}"
        except Exception as e:
            result = f"An unknown error occurred: {e}"


    return render_template('index.html', result=result)
>>>>>>> b86c1eff2970d6c386d838164ef2bd2b7e1a00cf

if __name__ == '__main__':
    app.run(debug=True)