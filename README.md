AUTOMATIC-METER-READING-SYSTEM
==============================


Automatic Meter Reading System is a single phase AC static power meter 

which makes use of the latest technology in the field of communication 

technology- GSM to send its power usage reading using Short Message 

System (SMS) back to electricity board and user wirelessly. The consumer as 

well as the utility company have constant access to consumption and status 

data, available on request, where the request is made by sending certain pre-
programmed commands (bill, on, off, etc) as SMSs to the corresponding SIM 

number of the GSM module interfaced with PIC18f4550 .It also displays the 

reading on GLCD. Also, a scheduler ensures that the electricity usage data 

at the end of each month is automatically dispatched to the utility provider 

simultaneously clearing the number of units consumed in that month stored in 

the EPROM

An external relay circuit incorporated permits the utility provider to cut 

off the supply through SMS, if the consumer fails to remit the bill. Even 

the consumer can perform this action; however it is strictly ensured that a 

message from the consumer aiming at unblocking the supply is ignored if the 

utility provider was responsible for cutting off the supply. In view of promoting 

power conservation, if the electrical usage of the consumer during peak hours 

exceeds 40 units, a scheduler formulated dispatches the consumer a message 

encouraging him to reduce power consumption.

Another highlight of the project is the inclusion of USB connection device 

which facilitates communication with the computer. This feature enables us 

to change the SIM number of the consumer during instances when he changes 

the SIM. Thus, the project presents a more efficient means of meter reading 

mechanism, where a communication technique automatically collects the 

meter readings and other relevant data from electric meters, thus saving 

physical visits to the place where the meter is installed. Despite of increased 

performance in data collection, errors associated with manual data entry 

or transfer is eliminated. Expenses arising out of meter reading will be 

significantly diminished, and its financial benefits last over the lifetime of the 

AMR system. Improved customer service is also achieved.
