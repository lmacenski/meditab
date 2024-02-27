## Meditab is a simple esp8266 web based pill dispenser that allows doctors to remotely alter dosage and timing of pill dispensing, while ensuring an easy end user experience.

**This project (in its current state) was made in under 24 hours for the 2024 Revolution UC hackathon. We hope this project can inspire similar projects
**
### **Patient View:**
Leveraging the capabilities of an ESP8266, which interfaces with the Kintone API every minute, we have developed a device that can accurately administer medication at specified intervals. The Kintone API allows physicians to swiftly adjust both the timing and dosage of each administration. Upon continuing this project, we envision designing a tablet interface to enhance patient notifications, allow the user to chat with their doctors and provide a simple line of contact between the doctor and patient. 

Our device currently utilizes an stepper motor in combination with an esp8266 to accurately take one pill from a gravity fed system and dispense them into an easy to access tray.
![alt text](https://github.com/lmacenski/meditab/blob/main/Assets/topview.jpg?raw=true)
![alt text](https://github.com/lmacenski/meditab/blob/main/Assets/sideview.jpg?raw=true)


### **Doctor View:**
Using the kintone API and database system, a doctor can quickly change the times and pill dosages for multiple patients. Each device we have designed calls the api with an unique ID (Record number) allowing the assignment of multiple dispensers per user. The code is scalable, so more dosage times could easily be added.  
![alt text](https://github.com/lmacenski/meditab/blob/main/Assets/Kintone.png?raw=true)


