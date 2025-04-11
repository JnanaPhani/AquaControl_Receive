# AquaControl_Receive

This is Receiver code in AquaControl project
 
Features Implemented 

* Lora intialization
* Auto Motor On / Off 
* Manual Motor On / Off
* If Lora Connected LED 02 will on 
* If Lora Not Connected LED 02 will be off
* If Water Level is Less than 40% LED 01 will be on 
* If Water Level is Greater than 41% LED 01 will be off
* If Relay_Pin is on LED 04 will toggle
* If Relay_Pin is off LED 04 will off 
* If Manual Switch is Pressed once Relay_Pin should on
* If Manual Switch is Pressed again Relay_Pin should off

Pin Configuration
---------------------------------
|GPIO 		|ESP		|
---------------------------------
|LORA_RST	|IO5		|
|LORA_MISO	|IO12		|
|LORA_MOSI	|IO13		|
|LORA_SCK	|IO14		|
|LORA_NSS	|IO15		|
|LORA_DIO	|IO34		|
|D4		|IO18		|
|D3		|IO19		|
|D2		|IO21		|
|D1		|IO22		|
|GPIO_S		|IO27		|
---------------------------------
