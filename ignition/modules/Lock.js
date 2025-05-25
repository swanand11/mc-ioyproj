const { buildModule } = require("@nomicfoundation/hardhat-ignition/modules");

module.exports = buildModule("MsgModule", (m) => {
  const msg = m.contract("Msg");  // Must match contract name exactly

  return { msg };
});
