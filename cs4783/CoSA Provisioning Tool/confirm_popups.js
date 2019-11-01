
$(document).on('click', '#btn-submit', function (e) {
    e.preventDefault();
    var EMP_FIRST = document.getElementById("EMP_FIRST").value
    var EMP_LAST = document.getElementById("EMP_LAST").value
    var content = document.createElement('div');
    content.innerHTML = '<strong>' + EMP_FIRST + " " + EMP_LAST + '</strong>';

    swal({
        title: "Confirmation",
        text: "Submit for employee? ",
        //icon: "warning",
        content, content,
        buttons: true,
    })
        .then((result) => {
            if (result) {
                swal({
                    title: "Form Submitted!",
                    //text: "Submit for employee? ",
                    icon: "success",
                    content, content,

                }).then((result) => {
                    if (result) {
                        window.location.href = "http://localhost:3306/prov_main";
                    }
                });
                $('#EMP').submit();
            } else {
                swal("Cancelled form submission");
            }
        });
});



$(document).on('click', '#btn-reject', function (e) {
    e.preventDefault();

    swal({
        title: "Reject form?",
        //text: "Reject form? ",
        icon: "warning",
        content: "input",
        buttons: true,
    })
        .then((result) => {
            if (result) {
                swal({
                    title: "Form rejected!",
                    //text: "Submit for employee? ",
                    icon: "error",
                    //content, content,

                }).then((result) => {
                    if (result) {
                        window.location.href = "http://localhost:3306/prov_main";
                    }
                });
                $('#EMP').append("<input type='hidden' name='REJECTED_COMMENT' value='" + result + "' />");
                $('#EMP').submit();
            } else {
                swal("Cancelled form rejection");
            }
        });
});






$(document).on('click', '#btn-approved', function (e) {
    e.preventDefault();
    var EMP_FIRST = document.getElementById("EMP_FIRST").value
    var EMP_LAST = document.getElementById("EMP_LAST").value
    var content = document.createElement('div');
    content.innerHTML = '<strong>' + EMP_FIRST + " " + EMP_LAST + '</strong>';

    swal({
        title: "Approve Form?",
        //text: "Submit for employee? ",
        //icon: "warning",
        content, content,
        buttons: true,
    })
        .then((result) => {
            if (result) {
                swal({
                    title: "Form Approved!",
                    //text: "Submit for employee? ",
                    icon: "success",
                    //content, content,

                }).then((result) => {
                    if (result) {
                        window.location.href = "http://localhost:3306/prov_main";
                    }
                });
                $('#EMP').append("<input type='hidden' name='submitValue' value='Submit' />");

                $('#EMP').submit();
            } else {
                swal("Cancelled form approval");
            }
        });
});
