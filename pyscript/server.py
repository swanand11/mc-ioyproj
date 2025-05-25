from flask import Flask, request, jsonify
from web3 import Web3
import json

app = Flask(__name__)

# Connect to local blockchain node
w3 = Web3(Web3.HTTPProvider("http://127.0.0.1:8545"))

# Load ABI
with open("pyscript/MsgABI.json") as f:
    abi_json = json.load(f)
abi = abi_json['abi']

# Contract address (update as needed)
contract_address = "0x5FbDB2315678afecb367f032d93F642f64180aa3"
contract = w3.eth.contract(address=contract_address, abi=abi)

# Use first account to send transactions
sender_account = w3.eth.accounts[0]

def is_chain_active():
    return w3.is_connected()

@app.route('/log', methods=['POST'])
def log_message():
    if not is_chain_active():
        return jsonify({"error": "Blockchain node not connected"}), 503

    data = request.json
    device_id = data.get("device_id")
    message = data.get("message")

    print(f"Chain active: {is_chain_active()}")
    print(f"Logging message: device_id={device_id}, message={message}")

    tx_hash = contract.functions.logMessage(device_id, message).transact({'from': sender_account})
    receipt = w3.eth.wait_for_transaction_receipt(tx_hash)

    print(f"Transaction successful with hash: {receipt.transactionHash.hex()}")

    return jsonify({
        "status": "Message logged on blockchain",
        "txHash": receipt.transactionHash.hex()
    })

if __name__ == '__main__':
    print("Starting server...")
    print("Chain active:", is_chain_active())
    app.run(host='0.0.0.0', port=5000, debug=True)
