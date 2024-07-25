from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/data', methods=['POST'])
def receive_data():
    try:
        json_data = request.get_json()
        if json_data is None:
            return jsonify({"error": "Invalid JSON"}), 400
        
        # 打印接收到的JSON数据
        print(json_data)
        
        # 解析并打印具体字段
        node_num = json_data.get('node_num')
        data = json_data.get('data', {})
        O2 = data.get('O2')
        CO = data.get('CO')
        H2S = data.get('H2S')
        CH4 = data.get('CH4')
        temp_int = data.get('temp_int')
        temp_dec = data.get('temp_dec')
        humi_int = data.get('humi_int')
        humi_dec = data.get('humi_dec')
        time = json_data.get('time')
        ip = json_data.get('IP')
        
        print(f"Node Number: {node_num}")
        print(f"O2: {O2}, CO: {CO}, H2S: {H2S}, CH4: {CH4}")
        print(f"Temperature: {temp_int}.{temp_dec}°C")
        print(f"Humidity: {humi_int}.{humi_dec}%")
        print(f"Time: {time}, IP: {ip}")
        
        return jsonify({"status": "success"}), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
