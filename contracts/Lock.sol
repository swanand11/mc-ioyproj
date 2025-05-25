// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

contract Msg {
    struct Message {
        string deviceId;
        string content;
        uint256 timestamp;
    }

    Message[] public messages;

    event MessageLogged(string deviceId, string content, uint256 timestamp);

    function logMessage(string memory _deviceId, string memory _content) public {
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
