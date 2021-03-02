void mqtt_reconnect() {
  // Loop until we're reconnected
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    //if (client.connect("ESP8266Client")) {
    if (client.connect("Thermostat", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mode_topic);
      client.subscribe(prog_topic);
      client.subscribe(stat_topic);
      client.subscribe(temp_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 5 seconds before retrying
      //delay(2000);
    }
  }
}

void mqtt_update_all() {
  client.publish(temperature_topic, String(t_intTemp).c_str(), true);
  client.publish(humidity_topic, String(h_intHum).c_str(), true);
  client.publish(stat_topic, String(g_sts).c_str(), true);
  client.publish(mode_topic, modes[g_mode].c_str(), true);
  client.publish(prog_topic, progs[g_prog].c_str(), true);
  client.publish(range_topic, String(c_timeline).c_str(), true);
  client.publish(fire_topic, String(g_fire).c_str(), true);
  client.publish(temp_topic, String(t_adjust).c_str(), true);
}

void mqtt_update(int idx) {
  switch (idx) {
    case 0:
      client.publish(temperature_topic, String(t_intTemp).c_str(), true);
      client.publish(humidity_topic, String(h_intHum).c_str(), true);
      client.publish(stat_topic, String(g_sts).c_str(), true);
      client.publish(mode_topic, modes[g_mode].c_str(), true);
      client.publish(prog_topic, progs[g_prog].c_str(), true);
      client.publish(range_topic, String(c_timeline).c_str(), true);
      client.publish(fire_topic, String(g_fire).c_str(), true);
      client.publish(temp_topic, String(t_adjust).c_str(), true);
      break;
    case 1:
      client.publish(mode_topic, modes[g_mode].c_str(), true);
      break;
    case 2:
      client.publish(prog_topic, progs[g_prog].c_str(), true);
      break;
    case 3:
      client.publish(temperature_topic, String(t_intTemp).c_str(), true);
      client.publish(humidity_topic, String(h_intHum).c_str(), true);
      break;
    case 4: //status
      client.publish(stat_topic, String(g_sts).c_str(), true);
      break;
    case 5: //fire
      client.publish(fire_topic, String(g_fire).c_str(), true);
      break;
    case 6: //manual temp adjust
      client.publish(temp_topic, String(t_adjust).c_str(), true);
      break;
  }

}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  if (strcmp(topic, mode_topic) == 0) {
    for (int i = 0; i < sizeof(modes); i++) {
      if (messageTemp == modes[i]) {
        g_mode = i;
      }
    }
    f_set_mode();
  } else if (strcmp(topic, prog_topic) == 0) {
    for (int i = 0; i < sizeof(progs); i++) {
      if (messageTemp == progs[i]) {
        g_prog = i;
      }
    }
    f_set_prog();
  } else if (strcmp(topic, stat_topic) == 0) {
    g_sts = messageTemp.toInt();
    f_set_sts();
  } else if (strcmp(topic, temp_topic) == 0) {
    t_adjust = messageTemp.toFloat();
    f_set_adjust();
  }
}


bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) &&
         (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}
