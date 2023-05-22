String tag_id = "";

bool FDXB_check() {
  if (Serial2.available() > 0) {
    int val = Serial2.read();
    writeInt(val);

    tag_id += String(val);

  }

  if (tag_id.length() == 30) {
    Serial.println("Tag Found : " + tag_id);
    tag_id = "";

    return true;
  }

  return false;
}