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