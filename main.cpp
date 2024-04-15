#define CROW_MAIN

#include "crow_all.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace crow;

//thse sets of functions allow me to render the pages efficiently through crow
void sendStyles(response&, string); //for css files
void sendHtml(response&, string); //for html files
void sendJScript(response&, string); //for javascript files
void sendJPEG(response&, string); //for jpeg images
void sendFile(response&, string, string, string); //the generic umbrella function that can be called by evrything above

//generic function that just displays the files
void sendFile(response& res, string path, string filename, string contentType)
{
	string filePath = path + filename; //gets a relative file path through simple string arithmetic
	ifstream readFile(filePath, ifstream::in); //read file contents

	if (readFile) {
		ostringstream contents; //stream file contents into 1 string with ostringstream 
		contents << readFile.rdbuf(); 
		readFile.close(); //close file

		res.set_header("Content-Type", contentType); //sets the header content type to allow crow render the page accurately
		res.write(contents.str()); //writes file contents (in string variable) to browser 
	}
	else { //if something goes wrong
		res.code = 404; //send this code
		res.set_header("Content-Type", "text/html");
		res.write("<h1> 404 Page did not open </h1>"); //write this error message to browser
	}
	res.end(); //end automatically sends to the client
}


void sendStyles(response& res, string filename) //for css files
{
	sendFile(res, "../public/styles/", filename, "text/css"); //MIME standard content type for css files
}

void sendHtml(response& res, string filename) //for html files
{
	sendFile(res, "../public/", filename, "text/html"); //MIME standard content type for html files
}

void sendJScript(response& res, string filename) //for javascript files
{
	sendFile(res, "../public/scripts/", filename, "application/javascript"); //MIME standard content type for javascript files
}

void sendJPEG(response& res, string filename) //for jpeg images
{
	sendFile(res, "../public/images/", filename, "image/jpeg"); //MIME standard content type for image files
}


//Lab 10 specific functions
void ToCheckout(response&, string, string);
void ToCheckout(response& res, string filename, string contentType) {
	string filepath = "../public/" + filename;
	ifstream readFile(filepath, ifstream::in); //read file contents

	if (readFile) {
		ostringstream contents; //stream file contents into 1 string with ostringstream 
		contents << readFile.rdbuf(); 
		readFile.close(); //close file

		res.set_header("Content-Type", contentType); //sets the header content type to allow crow render the page accurately
		res.code = 402; //payment required (requests login information)
		res.write(contents.str()); //writes file contents (in string variable) to browser 
	}
	else { //if something goes wrong
		res.code = 404; //send this code
		res.set_header("Content-Type", "text/html");
		res.write("<h1> 404 Page did not open </h1>"); //write this error message to browser
	}

	res.end(); //end automatically sends to the client	
}

int STATE = 0; //for cart state
int main()
{
	crow::SimpleApp app;
	string cartFile = "cart.txt"; //shopping cart

	CROW_ROUTE(app, "/") //root path
		([](const request &req, response &res) {
		sendHtml(res, "index.html");
		});

	CROW_ROUTE(app, "/scripts/<string>") //renders javascript files 
		([](const request& req, response& res, string filename) {
		sendJScript(res, filename);
		});

	CROW_ROUTE(app, "/<string>") //renders html files 
		([](const request& req, response& res, string filename) {
		sendHtml(res, filename);
		});

	CROW_ROUTE(app, "/styles/<string>") //renders css files 
		([](const request &req, response &res, string filename) {
		sendStyles(res, filename);
		});

	CROW_ROUTE(app, "/images/<string>") //renders jpeg image files 
		([](const request& req, response& res, string filename) {
		sendJPEG(res, filename);
		});

	CROW_ROUTE(app, "/Checkout/<string>") 
		([](const request& req, response& res, string filename) {
		string contentType = "text/html";
		ToCheckout(res, filename, contentType);
		});

	CROW_ROUTE(app, "/login").methods(HTTPMethod::POST)
		([&](const request& req, response& res) {
		string UserName = req.url_params.get("Username");
		string Password = req.url_params.get("Password");
		if (!UserName.empty() && !Password.empty()) {
			if(UserName == "Nelly" && Password == "1234")
			{
				res.code = 202;
				res.write("<script>alert('Purchase confirmed! Thank you for shopping at Candle Shop');</script>"); //sent javascript message box code to confirm purchase

				if(STATE == 7){
					ifstream readCartFile(cartFile); //open cart file for reading
					ostringstream streamCartFile;
					if(readCartFile.is_open()){ //read cart satae from file
						streamCartFile << readCartFile.rdbuf();
						readCartFile.close();
						res.write(streamCartFile.str()); //return cart state from ASCII file to plain text
					}
					else{ //handles errors
						res.code = 500;
						res.set_header("Content-Type", "text/html");
						res.write("<h1> 500 Server could not read cart state </h1>");
					}
				}
			}
			else
			{
				res.code = 401;
				res.set_header("Content-Type", "text/html");
				res.write("<h1> 401 You do not exist in our database </h1>");
			}
			
			res.end();
		}
	});
	//to prevent myself from hardcoding the cart.html route, I modified
	//REQ04 so that it will open the cart.html. That is why there
	//is an extra <string> at the end
	CROW_ROUTE(app, "/AddToCart/<int>/<string>/<string>") 
		([&](const request& req, response& res, int quantity, string product, string filename) {
		ofstream writeToCart(cartFile); //save state to shopping cart file
		STATE = quantity;
		if (STATE == 0){
			res.code = 200;
			if(writeToCart.is_open()){
				res.code = 200;
				writeToCart << "Cart is empty" << endl;
			}
			else
			{
				res.code = 500;
			}
		}
		else{
			if(writeToCart.is_open()){
				STATE = 7; //7 represents product is on queue for shipment
				writeToCart << "The cart state is: " << STATE << ". Means that your " << quantity << " items(s) of " << product << " is on queue for shipment" << endl;
				res.code = 200;
			}
			else{
				res.code = 500;
				res.set_header("Content-Type", "text/html");
				res.write("<h1> 500 i couldn't open my file </h1>");
			}
		}
		writeToCart.close(); //close file after use
		sendHtml(res, filename); //render cart.html after saving cart state to file
		});

	

	app.port(23500).multithreaded().run();
	
	return 0;
}