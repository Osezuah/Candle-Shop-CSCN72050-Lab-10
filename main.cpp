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
		res.write("Page did not open"); //write this error message to browser
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
		res.write("Page did not open"); //write this error message to browser
	}
	res.end(); //end automatically sends to the client
}

int main()
{
	crow::SimpleApp app;

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

	//to prevent myself from hardcoding the cart.html route, I modified
	//REQ04 so that it will open the cart.html. That is why there
	//is an extra <string> at the end
	CROW_ROUTE(app, "/AddToCart/<int>/<string>/<string>") 
		([](const request& req, response& res, int quantity, string product, string filename) {
		sendHtml(res, filename);
		});

	CROW_ROUTE(app, "/Checkout/<string>") 
		([](const request& req, response& res, string filename) {
		string contentType = "text/html";
		ToCheckout(res, filename, contentType);
		});

	app.port(23500).multithreaded().run();
	
	return 0;
}