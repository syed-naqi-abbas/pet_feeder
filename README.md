# 🐾 Smart Pet Feeder

A Wi-Fi-enabled smart pet feeder built using **ESP32** and an **auger-based dispensing mechanism**. This project enables users to **dispense food instantly** or **schedule automatic dispensing** using the **Blynk mobile app**.

---

## 📦 Features

* 📱 **Mobile Control with Blynk App**
* ⏰ **Scheduled Feeding with Persistent Storage**
* 🔁 **Multi-threaded Firmware on ESP32**
* ⚙️ **Auger-Based Dispensing Mechanism**
* 🔋 Battery-Powered for Portability

---

## 🧠 How It Works

### 🔌 Software Architecture

* **Platform**: ESP32 with Blynk firmware

* **Connectivity**: Wi-Fi (configured via Blynk)

* **Main Functionalities**:

  1. **Instant Dispensing**: Triggered from the app by toggling a virtual pin.
  2. **Scheduled Dispensing**: Users set a time in the app; the ESP32 checks the time every minute and dispenses when it's due.

* **Multithreading**:

  * **Thread 1**: Runs the Blynk firmware loop to handle cloud communication.
  * **Thread 2**: Checks system time every minute to trigger scheduled feeding.

* **Persistent Scheduling**:

  * Dispensing time is stored in the **EEPROM** so it is retained even after power loss or reboot.

---

## 🛠️ Hardware Components

| Component       | Description                            |
| --------------- | -------------------------------------- |
| ESP32           | Wi-Fi-enabled microcontroller          |
| Stepper Motor   | Drives the auger to dispense food      |
| 12V Battery     | Powers the motor                       |
| Buck Converter  | Steps down voltage to power ESP32      |
| Auger Mechanism | CAD-designed, rotates to dispense food |

> 💡 CAD files are included in the `/hardware` folder.

---

## 📱 Blynk App Configuration

You can control the feeder using the Blynk mobile app:

* **V0 (Virtual Pin 0)** – Instant Dispense
* **V1 (Virtual Pin 1)** – Set Scheduled Time (e.g., in HHMM format)

Once the virtual pin value changes, a function on ESP32 triggers the appropriate action.

---

## 🔄 Workflow

1. User sets schedule or taps "Dispense Now" in the Blynk app.
2. ESP32 receives the command via cloud.
3. If scheduled, ESP32 stores the time in EEPROM.
4. Background thread checks time every minute.
5. When the time matches, the stepper motor is activated.
6. Food is dispensed through auger mechanism.

---

## 📸 Project Preview

<img src="https://github.com/syed-naqi-abbas/pet_feeder/blob/main/pet_feeder.jpg" height="48" width="48">
