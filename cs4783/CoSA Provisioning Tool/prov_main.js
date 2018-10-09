$(document).ready(function () {
    
    $("#toggleMe").click(function () {            
        $("#completed").toggle();
        $("#rejected").toggle();
        
        if ( $(this).val() == "View Rejected" ) {
            $(this).val("View Completed");
        } else if ( $(this).val() == "View Completed" ){
            $(this).val("View Rejected");
        }
    });
    
    // $("#toggleMe_2").click(function () {
        // $("#completed").toggle();
        // $("#rejected").toggle();
    // });
    
    $("#order,#approve").click(function () {
        $("#toggleMe").hide();
    });
    $("#history").click(function () {
        $("#toggleMe").show();
    });
    
});