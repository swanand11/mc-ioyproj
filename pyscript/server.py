from flask import Flask, request, jsonify
from web3 import Web3
import json

app = Flask(__name__)

# Connect to local blockchain node
w3 = Web3(Web3.HTTPProvider("http://127.0.0.1:8545"))

# Load ABI
with open("pyscript/MsgABI.json") as f:
    abi_json = json.load(f)
abi = abi_json["abi"]

# Contract address (from your deployment)
contract_address = "0x5FbDB2315678afecb367f032d93F642f64180aa3"
contract = w3.eth.contract(address=contract_address, abi=abi)
sender_account = w3.eth.accounts[0]

def is_chain_active():
    return w3.is_connected()

@app.route("/log", methods=["POST"])
def log_message():
    if not is_chain_active():
        return jsonify({"error": "Blockchain not connected"}), 503

    data = request.get_json()
    device_id = data.get("device_id")
    message = data.get("message")

    print(f"Incoming request: device_id={device_id}, message={message}")

    # Check if device is registered
    is_registered = contract.functions.registeredDevices(device_id).call()
    print(f"Device '{device_id}' registered? {is_registered}")

    if not is_registered:
        print(f"Registering device '{device_id}'...")
        tx_reg = contract.functions.registerDevice(device_id).transact({"from": sender_account, "gas": 300000})
        w3.eth.wait_for_transaction_receipt(tx_reg)
        print(f"Device '{device_id}' registered.")

    # Log the message
    print(f"Logging message from '{device_id}'...")
    tx_hash = contract.functions.logMessage(device_id, message).transact({"from": sender_account, "gas": 300000})
    receipt = w3.eth.wait_for_transaction_receipt(tx_hash)
    print(f"Message logged. TxHash: {receipt.transactionHash.hex()}")

    return jsonify({
        "status": "Message logged on blockchain",
        "txHash": receipt.transactionHash.hex()
    })

if __name__ == "__main__":
    print("Starting Flask server on port 5000...")
    print("Connected to chain:", is_chain_active())
    app.run(host="0.0.0.0", port=5000, debug=True)
