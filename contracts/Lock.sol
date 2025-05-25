// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract Msg {
    struct Message {
        string deviceId;
        string content;
        uint256 timestamp;
    }

    Message[] public messages;

    mapping(string => bool) public registeredDevices;

    event DeviceRegistered(string deviceId);
    event MessageLogged(string deviceId, string content, uint256 timestamp);

    // Register a new device (can be extended to include access control)
    function registerDevice(string memory _deviceId) public {
        require(!registeredDevices[_deviceId], "Device already registered");
        registeredDevices[_deviceId] = true;
        emit DeviceRegistered(_deviceId);
    }

    // Log message only if device is registered
    function logMessage(string memory _deviceId, string memory _content) public {
        require(registeredDevices[_deviceId], "Device not registered");

        Message memory newMsg = Message({
            deviceId: _deviceId,
            content: _content,
            timestamp: block.timestamp
        });

        messages.push(newMsg);
        emit MessageLogged(_deviceId, _content, block.timestamp);
    }

    function getMessageCount() public view returns (uint256) {
        return messages.length;
    }

    function getMessage(uint256 index) public view returns (string memory, string memory, uint256) {
        require(index < messages.length, "Index out of bounds");
        Message memory msgData = messages[index];
        return (msgData.deviceId, msgData.content, msgData.timestamp);
    }
}
