function addAmberToCart(){
    var selectElement = document.getElementById("AmberQuantity");
    var id1 = selectElement.id //gets the id from the select element
    var product = id1.split("Quantity")[0]; //gets the word 'Amber' from the id
    
    //make route
    var route = "/AddToCart/";
    route += selectElement.value; //integer value of quantity selected
    route += "/";
    route += product; //name of product selected
    route += "/";
    route += "cart.html"; 

    // Redirect to the route
    window.location.href = route;
}

function addFiresideToCart(){
    var selectElement = document.getElementById("FiresideQuantity");
    var id1 = selectElement.id //gets the id from the select element
    var product = id1.split("Quantity")[0]; //gets the word 'Fireside' from the id
    
    //make route
    var route = "/AddToCart/";
    route += selectElement.value; //integer value of quantity selected
    route += "/";
    route += product; //name of product selected
    route += "/";
    route += "cart.html"; 

    // Redirect to the route
    window.location.href = route;
}

function addWoodToCart(){
    var selectElement = document.getElementById("WoodQuantity");
    var id1 = selectElement.id //gets the id from the select element
    var product = id1.split("Quantity")[0]; //gets the word 'Fireside' from the id
    
    //make route
    var route = "/AddToCart/";
    route += selectElement.value; //integer value of quantity selected
    route += "/";
    route += product; //name of product selected
    route += "/";
    route += "cart.html"; 

    // Redirect to the route
    window.location.href = route;
}

function addBenevolenceToCart(){
    var selectElement = document.getElementById("BenevolenceQuantity");
    var id1 = selectElement.id //gets the id from the select element
    var product = id1.split("Quantity")[0]; //gets the word 'Fireside' from the id
    
    //make route
    var route = "/AddToCart/";
    route += selectElement.value; //integer value of quantity selected
    route += "/";
    route += product; //name of product selected
    route += "/";
    route += "cart.html"; 

    // Redirect to the route
    window.location.href = route;
}

function login(){
    var action1 = "/login?";
    var uname = document.getElementById("Uname").value; //Uname is username
    action1 += "Username=" + uname + "&";
    var pword = document.getElementById("Pword").value; //Pword is password
    action1 += "Password=" + pword;
    document.getElementById("Login").action = action1;
    document.getElementById("Login").submit(); 
}
