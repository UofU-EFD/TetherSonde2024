// Not really nessicary for our purposes but it is handy.
void sendMessage(String message) {
  LoRa.beginPacket();
  LoRa.println(message);
  LoRa.endPacket();
}