# breakgenerator
Generates breaks for Hannaford front end scheduling upon input of shift times and employee descriptions

Rules:

Shift is >= 4 hours -> break
	- If associate is under 18, a shift >= 3.5 hours -> break

Shift is >= 6 hours -> break + lunch

Shift is >= 7 hours -> break + lunch + break

Otherwise -> no break

Diane -> Manager, does not get scheduled breaks

- Breaks only overlap when absolutely necessary
	- Not perfect but appropriate logic placed on what initial overlapping break occurs first
		- Ex. 3:45 - 7:45 break occurs after 3:00 - 8:30 break since latter associate comes in first

- Breaks occur less than three hours in between end of shift/next break unless absolutely necessary

- Ex: Typical 8.5 hr shift (8 hr shift + lunch) -> break 2 hrs into shift, lunch 2 hrs after break, break 2.5 hrs after lunch

- Associates separated from cashier trained, non cashier trained, service leaders, and training
	- Training associates should have similar/same breaks to account for them working together

- All determined through input file consisting of associate name, day, and time of shifts for week
	- Can also be done one by one through console/shell
	- Goes in loop until 'q' or 'Q' detected
	1. If associate is not cashier trained, add 'y' followed by comma
	2. Enter associate name, followed by comma
	3. If associate is under 18, add 'y' followed by comma
	4. Enter shift day, either in full or abbreviated ("Monday" or "mo", etc.)
	5. If associate is a service leader, add 'y' followed by comma
	6. Enter starting time of shift, followed by comma
	7. Enter end time of shift, followed by comma
	8. If associate is currently training, add 'y'
	9. If other shifts exist for the associate within the week, add comma and repeat steps iv, and vi-ix until finished
	10. When completely finished, press q to quit and generate grids for week

See files for examples of input/output
	
