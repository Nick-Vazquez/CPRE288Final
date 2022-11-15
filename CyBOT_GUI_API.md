# CyBOT GUI Communication API
### CPR E 288 - F22 - 1_PG_4
#### _Nolan Eastburn, Ryan Lowe, Rachel San Augustin, Nick Vazquez_

### Overview:
The goal of this document is to define communication standards between the CyBOT
and the accompanying GUI application.

### Data encoding:
Data will be encoded in _JSON_ format, and will be sent over the communication 
socket in _bytes_. The JSON will be serialized or deserialized depending on the
direction of communication.

### Important notes
* Distances can be used interchangeably. The GUI will render both sensors in 
different styles and a single or both values can be updated in a message.

## Types of messages:
* Scan Messages
  * These messages are sent from the CyBOT and include updated information about
  a scan.
  * **Single scan update**
  ```json
  {
    "type": "scan",
    "angle": 120,
    "distance_ir_cm": 25,
    "distance_ping_cm": 25
  }
  ```
  
  * **Full scan update**
  ```json
  {
    "type": "full_scan",
    "angles": [120, 122, 124, 126, 128, 130],
    "distances_ir_cm": [25, 24, 24, 25, 25, 26]
  }
  ```
  
  * **Scan average update**
    * This scan type could allow for very quick scanning of the peripheral area
    with little to no lag. The bot would calculate the average distance over a
    sweeping angle and combine multiple scan values into section.
    * The `start_angle` and `sweep_angle` must sum to be <= 180 degrees.
  ```json
  {
    "type": "section_scan",
    "start_angle": 0,
    "sweep_angle": 30,
    "distance_ir_cm": 20,
    "distance_ping_cm": 20 
  }
  ```

* Control Messages
  * These messages are sent from the GUI to the CyBOT to control CyBot 
  functions. 
* Task Messages