# Simple Internet Simulation
## Project Information
### Authors: David Haighton, Josh Blondin
This project was created for the purposes of showcasing a simple internet 
simulation using Cadmium for Dr. Gabriel Wainer's SYSC5104 class.

This simulation involves 2 Coupled Models, which are the Client and the Cloud.
The Client is split into 2 atomic models. The Cloud is split into 3 atomic models
and one coupled model, being the Lambda. The Lambda is then split into 4 atomic
(2 unique) models. This can all be shown in the diagram below:

![Simple Internet Model](https://cdn.discordapp.com/attachments/1066838119279771748/1078024321915703306/image.png)

## Files

All source Files can be found in /include. In there are the following files:
 - ### APIGateway.h
   - This is the atomic model representing the REST API Gateway. It serves as a
   gateway between the client and the cloud.
 - ### Bucket.h
   - This is the atomic model representing the S3 Bucket. It responds to client
   web requests which allow the client to access the rest of the cloud service.
 - ### Client.h
   - This is the coupled model representing the client. It couples together the
   client dispatcher and the client generator. It acts as the client accessing 
   a web page.
 - ### ClientDispatcher.h
   - This is the atomic model representing a client dispatcher. It takes in
   incoming messages and forwards them to either the bucket or the API Gateway.
 - ### ClientGenerator.h
   - This is the atmoic model representing a client generator. It creates the
   messages which are requests to the bucket.
 - ### Cloud.h
   - This is the coupled model representing the cloud service. It couples together
   the Bucket, API Gateway, Lambda and Database. It acts as the web page which a client
   is attempting to access. 
 - ### Database.h
   - This is the atomic model representing the Database. It takes incoming requests
   and generates a proper response to send back through the executor
 - ### Dispatcher.h
   - This is the atomic model representing the Dispatcher. It takes incoming 
   requests and dispatches them to one of the executors.
 - ### Executor.h
   - This is the atomic model representing the Executor. It takes incoming
   requests from the dispatcher, sends them to the database, and then waits for
   the database to generate a response and sends that back to the dispatcher.
 - ### Lambda.h
   - This is the coupled model representing the Lambda. It couples together
   the dispatcher and all of the executors.
 - ### Internet.h
   - This is the coupled model representing the entire system. It couples
   together the client and the cloud. 
 - ### Packet.h
   - This is the message type that is being passed between all of the models.
   It can either be a request or a response. 
## Compiling and Running
### Compilation Steps
In order to compile, make sure Cadmium is Installed and placed in the same folder 
as this project (IE cadmium_v2 and simple_website_simulation should both be in
the same folder). Then, in the project directory, run a make command. This will
compile the program and create an executable file in the bin folder called
serverless_recipe_site_sim. 
### Running the Simulation
Run the executable file mentioned above (or if you are still in the root folder
of the project you can run ./bin/serverless_recipe_site_sim). This will run the
simulation and generate a log file in the root project folder. You can view this
log file in order to see how the simulation ran.
## Tests
Our project had software tests which are located in the test folder. These contain
test scripts which are used to test each of the atomic models. They can be ran by
...