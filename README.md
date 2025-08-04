AI-Powered IoT System for Plant Disease Detection and Automated Fertilizer Precision Spraying

[Watch a demo of the AI-powered IoT system in action on Google Drive](https://drive.google.com/file/d/1RguORhYpgQR2I4bWk6TB-wsKrEVuRj2A/view?usp=drive_link)

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

Detects plant diseases using real-time image classification

Automatically sprays fertilizer precisely at the diseased plant

Minimizes fertilizer usage and human labor

Operates in both manual and automatic modes for versatility

🧠 Proposed Solution
A smart robotic system that uses an ESP32-CAM module to scan plants for disease using a trained machine learning model. When a disease is detected, the ESP32-CAM sends coordinates to the main ESP32 board via Wi-Fi. The main board then activates a CoreXY mechanism to move a spray nozzle to the target location, triggers a fertilizer pump, and optionally alerts with a buzzer. The system resets and continues scanning the next plant.

🛠️ Technologies & Tools Used
Hardware
Microcontrollers: ESP32-CAM, ESP32 board

Motion Control: CoreXY platform, stepper motors, L298N motor driver

Actuators: Relay modules, water pump, buzzer

Sensors: IR sensors, soil moisture sensor

Input: Joystick module

Power: Power supply

Software
IDE: Arduino IDE

Programming Languages: C/C++

Machine Learning: Edge Impulse Studio (for training and deploying the leaf disease detection model)

Communication: Wi-Fi (for ESP32-CAM to ESP32 main board)

ML Framework
Edge Impulse: Used for training and deploying the leaf disease detection model, optimizing it for embedded devices.

🧱 System Components
ESP32-CAM: Captures plant images and runs the disease detection model.

ESP32 Main Board: Receives detection signals and controls movement and spraying.

CoreXY Mechanism: An X-Y gantry system that moves the sprayer to the target plant.

Stepper Motors: Control directional movement of the CoreXY platform.

L298N Motor Driver: Controls the stepper motors.

Relay Module: Controls the pump and buzzer.

Pump: Sprays fertilizer.

Buzzer: Alerts when disease is detected.

Joystick: For manual mode control of the CoreXY platform.

Soil Moisture Sensor: (Optional) Detects water level in soil for future irrigation control.

🔗 Communication Flow
ESP32-CAM captures an image of each plant.

The image is processed by the onboard ML model for disease classification.

If disease is detected, the ESP32-CAM sends data (either a disease flag or precise coordinates) over Wi-Fi to the main ESP32 board.

The ESP32 main board receives the signal and activates the CoreXY mechanism to position the sprayer.

The sprayer applies fertilizer through the relay-controlled pump.

The buzzer remains ON until spraying is completed.

⚙️ Modes of Operation
Manual Mode: The user can control the CoreXY platform's movement using the joystick.

Automatic Mode: The system autonomously scans each plant, detects diseases, and applies fertilizer as needed.

🔁 Algorithm Flow (Simplified)
graph TD
    A[Start Scan] --> B{ESP32-CAM Captures Image};
    B --> C{Image Processed by ML Model};
    C -- Disease Found --> D{Send Disease Signal/Coordinates to ESP32 Main Board via Wi-Fi};
    D --> E[Activate Buzzer];
    E --> F[Activate CoreXY to Move to Target];
    F --> G[Trigger Pump to Spray];
    G --> H[Turn off Buzzer after Spraying];
    H --> I[Move to Next Plant];
    I --> B;
    C -- No Disease Found --> I;

✅ Features
AI-powered plant disease detection for real-time analysis.

Wireless (Wi-Fi) ESP32-to-ESP32 communication for seamless data transfer.

CoreXY robotic movement for precision targeting of affected areas.

Low-cost and scalable solution, making it accessible for small and large-scale farmers.

Automated disease alert and response system.

Customizable scanning area and number of plants to adapt to various farm layouts.

🎯 Real-World Impact
Saves fertilizer by spraying only where needed, reducing waste and environmental impact.

Reduces manual labor and monitoring time, increasing efficiency for farmers.

Increases crop yield through early disease management and targeted treatment.

Can be scaled to large fields with enhanced navigation capabilities.

A valuable tool for precision agriculture and sustainable farming practices.

📍 Deployment Scenario
The system was tested on a set of 6 money plants arranged in a row using a custom-built CoreXY platform. The ESP32-CAM was mounted at the scanning end, while the sprayer, pump, and relay were mounted on the CoreXY gantry. The entire system operates autonomously once powered and initiated, demonstrating its self-sufficiency.

📦 Future Improvements
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
KOVIRI SANDEEP - Gmail: koviri.sandeep@gmail.com- LinkedIn : www.linkedin.com/in/kovirisandeep

Project Link: [[Link to this GitHub repository]](https://github.com/SandeepKoviri/AI-powered-IoT-Plant-Disease-Detector.git)
