AI-Powered IoT System for Plant Disease Detection and Automated Fertilizer Precision Spraying

[Watch a Full Video of the AI-powered IoT system in action on Google Drive](https://drive.google.com/file/d/1RguORhYpgQR2I4bWk6TB-wsKrEVuRj2A/view?usp=drive_link)

[Watch a Making video of the AI-powered IoT system in action on Google Drive](https://drive.google.com/file/d/1VeI0MQnWoe3blZJvuf_1zh6At3UTlhc7/view?usp=drive_link)

📝 Project Summary
This project is an innovative fusion of IoT, AI, and embedded systems, designed to automate the process of detecting plant diseases and applying fertilizers precisely. It addresses the limitations of manual crop monitoring and fertilizer application by using real-time disease detection and targeted spraying, making agriculture smarter and more sustainable.

🔍 Problem Statement
Traditional farming relies heavily on manual inspection of crops for diseases and blanket fertilizer application, leading to:

Delay in disease detection

Excessive fertilizer usage

Increased labor cost

Lower crop yield

💡 Objective
To build an AI-enabled IoT system that:

Detects plant diseases using real-time image classification.

Automatically sprays fertilizer precisely at the diseased plant.

Minimizes fertilizer usage and human labor.

Operates in both manual and automatic modes for versatility.

🧠 Proposed Solution
A smart robotic system that uses an ESP32-CAM module to scan plants for disease using a trained machine learning model. When a disease is detected, the ESP32-CAM sends coordinates to the main ESP32 board via Wi-Fi. The main board then activates a CoreXY mechanism to move a spray nozzle to the target location, triggers a fertilizer pump, and alerts with a buzzer. The system further integrates Blynk Cloud for remote monitoring, control, and real-time alerts, providing a comprehensive solution for precision agriculture. The system resets and continues scanning the next plant.

⚙️ Detailed Working Principle (A to Z)
Image Acquisition: An ESP32-CAM module continuously captures live video/images of plant leaves as it moves across rows of plants.

AI-Based Disease Detection: Captured images are analyzed using a lightweight ML model deployed on the ESP32-CAM (trained with Edge Impulse). The model identifies whether a plant is healthy or diseased based on visible symptoms on the leaves.

Coordinate Communication: Once a disease is detected, the ESP32-CAM transmits data via Wi-Fi to a main ESP32 controller. The data includes the detection result and the relative coordinates of the diseased plant.

CoreXY Movement Control: The main ESP32 controls a CoreXY mechanism using stepper motors and drivers to precisely move a fertilizer-spraying nozzle to the diseased plant's location.

Fertilizer Spraying: A relay-activated pump sprays fertilizer precisely onto the detected plant using a nozzle controlled by the CoreXY system.

Siren Alert: A buzzer siren is triggered the moment disease is detected. It remains active until the spraying process is completed, then it automatically stops.

Moisture Monitoring (Optional Extension): A soil moisture sensor can also be integrated to monitor watering needs alongside fertilizer spraying.

Remote Monitoring with Blynk Cloud: All important parameters and events (like disease detected, pump on/off, coordinates of diseased plants, system status) are updated to the Blynk Cloud via the main ESP32 board.

Blynk Mobile Dashboard: Users can access the Blynk mobile app to monitor system operations in real time. The dashboard displays sensor data, disease detection alerts, current coordinates, pump status, and system mode (manual/automatic). Users can also switch modes and trigger actions remotely.

Power and Connectivity: The system operates using a 5V–12V power source. Wi-Fi connectivity is essential for communication between ESP32-CAM and the main ESP32, and for syncing with the Blynk Cloud.

Manual and Auto Modes: The system supports both:

Auto Mode: ESP32-CAM scans and triggers spraying autonomously.

Manual Mode: Using Blynk or a joystick, users can manually control the movement and spraying.

Application Scope:

Home gardening

Greenhouses

Vertical farms

Educational agriculture research

Precision farming test beds

🛠️ Technologies & Tools Used
Hardware
Microcontrollers: ESP32-CAM, ESP32 board

Motion Control: CoreXY platform, stepper motors, L298N motor driver

Actuators: Relay modules, water pump, buzzer

Sensors: DHT11 sensor (for humidity and temperature), soil moisture sensor

Input: Joystick module

Power: Power supply

Software
IDE: Arduino IDE

Programming Languages: C/C++

Machine Learning: Edge Impulse Studio (for training and deploying the leaf disease detection model)

IoT Platform: Blynk Cloud

Communication: Wi-Fi (for ESP32-CAM to ESP32 main board and Blynk Cloud)

🧱 System Components
ESP32-CAM: Captures plant images and runs the disease detection model.

ESP32 Main Board: Receives detection signals, controls movement and spraying, and communicates with Blynk Cloud.

CoreXY Mechanism: An X-Y gantry system that moves the sprayer to the target plant.

Stepper Motors: Control directional movement of the CoreXY platform.

L298N Motor Driver: Controls the stepper motors.

Relay Module: Controls the pump and buzzer.

Pump: Sprays fertilizer.

Buzzer: Alerts when disease is detected.

Joystick: For manual mode control of the CoreXY platform.

Soil Moisture Sensor: Detects water level in soil for future irrigation control.

DHT11 Sensor: For Humidity and Temperature near Plants.

Blynk Cloud and Mobile App: For remote monitoring, control, and alerts.

🔁 Algorithm Flow (Simplified)
graph TD

    A[Start Scan] --> B{ESP32-CAM Captures Image};
    B --> C{Image Processed by ML Model};
    C -- Disease Found --> D{Send Disease Signal/Coordinates to ESP32 Main Board via Wi-Fi};
    D --> E[Activate Buzzer];
    E --> F[Activate CoreXY to Move to Target];
    F --> G[Trigger Pump to Spray];
    G --> H[Turn off Buzzer after Spraying];
    H --> I[Update Blynk Cloud with Status];
    I --> J[Move to Next Plant];
    J --> B;
    C -- No Disease Found --> J;

✅ Features
AI-powered plant disease detection for real-time analysis.

Wireless (Wi-Fi) ESP32-to-ESP32 communication for seamless data transfer.

CoreXY robotic movement for precision targeting of affected areas.

Remote monitoring and control via Blynk Cloud and mobile app.

Low-cost and scalable solution, making it accessible for small and large-scale farmers.

Automated disease alert and response system.

Customizable scanning area and number of plants to adapt to various farm layouts.

Dual modes of operation (Manual and Automatic) for enhanced versatility.

🎯 Real-World Impact
Saves fertilizer by spraying only where needed, reducing waste and environmental impact.

Reduces manual labor and monitoring time, increasing efficiency for farmers.

Increases crop yield through early disease management and targeted treatment.

Can be scaled to large fields with enhanced navigation capabilities.

A valuable tool for precision agriculture and sustainable farming practices.

📍 Deployment Scenario
The system was tested on a set of 6 money plants arranged in a row using a custom-built CoreXY platform. The ESP32-CAM was mounted at the scanning end, while the sprayer, pump, and relay were mounted on the CoreXY gantry. The entire system operates autonomously once powered and initiated, with remote monitoring and control capabilities via Blynk, demonstrating its self-sufficiency.

📦 Project Structure
A brief overview of your directory structure:

/src

    /ai_model        # Trained AI models and Edge Impulse project files
    /firmware        # C/C++ code for ESP32-CAM and ESP32 main board (Arduino IDE projects)
    /blynk           # Blynk project setup (e.g., dashboard configuration, virtual pins)
/docs

    /schematics      # Circuit diagrams and wiring guides
    /datasheets      # Component datasheets
/media

    /photos          # Project photos
    /videos          # Demo videos
README.md

🚀 Getting Started
Instructions on how someone could replicate your project:

Clone the repository: git clone https://github.com/SandeepKoviri/AI-powered-IoT-Plant-Disease-Detector.git

Hardware Setup:

Assemble the CoreXY mechanism.

Wire the ESP32-CAM, ESP32 main board, stepper motors, L298N drivers, relay, pump, buzzer, joystick, DHT11, and soil moisture sensor as per the schematics in  
/docs/schematics.

Ensure proper power supply connections (5V-12V).

Software Setup:

Arduino IDE: Install Arduino IDE and add ESP32 board support. Install necessary libraries (e.g., for Wi-Fi, stepper motors, Blynk, camera, DHT sensor).

Edge Impulse: Set up your Edge Impulse project for leaf disease detection. Train and deploy your ML model to the ESP32-CAM.

Blynk Cloud: Create a new project in Blynk Cloud. Design your mobile dashboard with appropriate widgets (buttons, displays, gauges) and link them to virtual pins used in your ESP32 firmware. Obtain your Blynk authentication token.

Upload Firmware:

Open the ESP32-CAM firmware project (from /src/firmware/esp32_cam_firmware) in Arduino IDE. Update Wi-Fi credentials.

Open the ESP32 main board firmware project (from /src/firmware/esp32_main_firmware) in Arduino IDE. Update Wi-Fi credentials and Blynk authentication token.

Upload the respective firmware to your ESP32-CAM and main ESP32 boards.

Run the System:

Power on the entire system.

Connect your Blynk mobile app to the project using your authentication token.

Observe the system in action, either in Manual or Automatic mode, and monitor data on your Blynk dashboard.

🔮 Future Improvements
Advanced Navigation: Add GPS or AI-based navigation for open field use, allowing deployment in larger, unstructured environments.

Long-Range Communication: Integrate LoRa for long-range communication, extending the system's reach in vast agricultural areas.

Sustainable Energy: Add a solar panel for sustainable energy, making the system self-sufficient and eco-friendly.

Expanded ML Model: Expand the ML model to detect multiple crop types and a wider range of diseases.

Integrated Irrigation: Integrate soil moisture-based irrigation control for a complete plant care solution.

📌 Project Duration
Development: ~3 months

Testing & Deployment: ~1 week

Total: 4 months

🤝 Contact
KOVIRI SANDEEP

Gmail: koviri.sandeep@gmail.com

LinkedIn: www.linkedin.com/in/kovirisandeep

Project Link: https://github.com/SandeepKoviri/AI-powered-IoT-Plant-Disease-Detector.git
