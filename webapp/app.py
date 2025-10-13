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

if __name__ == '__main__':
    app.run(debug=True)