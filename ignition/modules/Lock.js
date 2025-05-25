const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

module.exports = buildModule("MsgModule", (m) => {
  const msg = m.contract("Msg");

  // Register two devices with unique IDs for each call
  m.call(msg, "registerDevice", ["esp32_sender"], { id: "registerSender" });
  m.call(msg, "registerDevice", ["esp32_receiver"], { id: "registerReceiver" });

  return { msg };
});
