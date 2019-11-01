const express = require('express');
const mysql = require('mysql');
const app = express();
const bodyParser = require("body-parser");
const ejs = require("ejs");
const moment = require('moment');
const session = require('express-session');
const $ = require('jquery');
const MySQLStore = require('express-mysql-session')(session);

// Application initialization
var connection = mysql.createConnection({
	host: 'easel1.fulgentcorp.com',
	user: 'autobean_user',
	password: 'z@73gt53#fZwM6!PfRL',
	database: 'autobean'
});

app.listen(3306);
//parses information ?somehow?
app.use(bodyParser.urlencoded({
	extended: true
}));

//make it so express can read ejs files
app.set('view engine', 'ejs');

//setting up a session
app.use(session({ secret: 'lg43gsdhjklliyt453h1ghj98l2lj1fv34m2n87m2ciz24x1cxx34c214654m' }));
//makes it so you logout and can't hit the back button to go to previous pages.
app.use(function (req, res, next) {
	res.set('Cache-Control', 'no-cache, private, no-store, must-revalidate, max-stale=0, post-check=0, pre-check=0');
	next();
});

//This call loads ALL files (js, html, css)
//__dirname is always the directory in which the currently executing script resides
app.use(express.static(__dirname));
app.use(express.static('views'));


//Now that the files are loaded they can be called with an app.get
//      ↓↓↓↓↓↓↓↓↓↓↓ this will be where the file is displayed so prov_add.html will appear at http://localhost:3306/prov_add
//5.2 changed to res.render
app.get('/prov_add', function (req, res) {
	if (req.session.username) {
		res.render('prov_add');
	} else {
		loginRedirect(res)
	}
});

// 5.6

app.use('/prov_main', function (req, res) {
	if (req.session.REQUESTOR == "1") {
		display_requestor_history(res, req)
	} else if (req.session.FISC_ADMIN == "1") {
		display_fisc_admin_history(res, req)
	} else if (req.session.DEPT_HEAD == "1") {
		display_dept_head_history(res, req)
	} else {
		loginRedirect(res)
	}
});

function display_requestor_history(res, req) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID, FORM_CREATION_TIME FROM masterform WHERE REQUESTOR = ? ORDER BY EMP_LAST"
	var values = "1";

	connection.query(sql, [values], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)

		var sql_arch = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
		FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID, FORM_CREATION_TIME FROM historyform WHERE REQUESTOR = ? ORDER BY EMP_LAST"
		var values = "approved";

		connection.query(sql_arch, [values], function (err, result, fields) {
			if (err) throw err;
			var varFormObject_arch = result;

			var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM masterform WHERE CREATION_NAME = ? ORDER BY EMP_LAST"
			var values = req.session.FIRST_NAME;

			connection.query(sql, [values], function (err, result, fields) {
				if (err) throw err;
				var formObjects_name = result;

				var sql = "SELECT * FROM rejectedform ORDER BY EMP_LAST"
				connection.query(sql, [values], function (err, result, fields) {
					if (err) throw err;
					var formRejected = result;

					res.render('prov_main', {
						formObjects: formObjects,
						varFormObject_arch: varFormObject_arch,
						formObjects_name: formObjects_name,
						formRejected: formRejected
					});
				});
			});
		});
	});
}

function display_fisc_admin_history(res, req) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID, FORM_CREATION_TIME FROM masterform WHERE FISC_ADMIN = ? ORDER BY EMP_LAST"
	var values = "1";

	connection.query(sql, [values], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)

		var sql_arch = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
		FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID, FORM_CREATION_TIME FROM historyform WHERE FISC_ADMIN = ? ORDER BY EMP_LAST"
		var values = "approved";

		connection.query(sql_arch, [values], function (err, result, fields) {
			if (err) throw err;
			var varFormObject_arch = result;



			var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM masterform WHERE CREATION_NAME = ? ORDER BY EMP_LAST"
			var values = req.session.FIRST_NAME;


			connection.query(sql, [values], function (err, result, fields) {
				if (err) throw err;
				var formObjects_name = result;

				var sql = "SELECT * FROM rejectedform ORDER BY EMP_LAST"
				connection.query(sql, [values], function (err, result, fields) {
					if (err) throw err;
					var formRejected = result;


					res.render('prov_main', {
						formObjects: formObjects,
						varFormObject_arch: varFormObject_arch,
						formObjects_name: formObjects_name,
						formRejected: formRejected
					});
				});
			});
		});
	});
}

function display_dept_head_history(res, req) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
	FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID, FORM_CREATION_TIME \
	FROM masterform WHERE DEPT_HEAD = ? AND REQUESTOR = ? AND FISC_ADMIN = ? ORDER BY EMP_LAST"

	connection.query(sql, ["1", "approved", "approved"], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)

		var sql_arch = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
		FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID, FORM_CREATION_TIME FROM historyform WHERE REQUESTOR = ? ORDER BY EMP_LAST"
		var values = "approved";

		connection.query(sql_arch, [values], function (err, result, fields) {
			if (err) throw err;
			varFormObject_arch = result;
			var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM masterform WHERE CREATION_NAME = ? ORDER BY EMP_LAST"
			var values = req.session.FIRST_NAME;

			connection.query(sql, [values], function (err, result, fields) {
				if (err) throw err;
				var formObjects_name = result;

				var sql = "SELECT * FROM rejectedform ORDER BY EMP_LAST"
				connection.query(sql, [values], function (err, result, fields) {
					if (err) throw err;
					var formRejected = result;


					res.render('prov_main', {
						formObjects: formObjects,
						varFormObject_arch: varFormObject_arch,
						formObjects_name: formObjects_name,
						formRejected: formRejected
					});
				});
			});
		});
	});
}


//5.6
app.get('/archive_forms', function (req, res) {

	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM historyform WHERE REQUESTOR = ? ORDER BY EMP_LAST"
	var values = "approved";

	connection.query(sql, [values], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)
		var sql = "SELECT * FROM rejectedform ORDER BY EMP_LAST"
		console.log("@@@@@@@@@@@@@@@@@" + typeof formObjects);


		connection.query(sql, function (err, result, fields) {
			if (err) throw err;
			var formObjects2 = result;


			res.render('archive_forms', {
				formObjects: formObjects,
				formObjects2: formObjects2
			});
		});
	});
});



//5.2
//add a completion time
app.get('/bes_status', function (req, res) {
	if (req.session.BES) {
		display_bes(res);
	}
});

function display_bes(res) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM historyform WHERE FORM_FUEL = ? ORDER BY EMP_LAST"
	var values = "on";
	connection.query(sql, [values], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		res.render('BES_status', {
			formObjects: formObjects
		});
	});
}

//5.6


app.get('/itsd_status', function (req, res) {
	if (req.session.ITSD) {
		display_itsd(res);
	}
});

function display_itsd(res) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM historyform WHERE FORM_IDCARD = ? OR FORM_HW_SW = ? OR FORM_USER_PROV = ? ORDER BY EMP_LAST"
	var values = "on";
	connection.query(sql, [values, values, values], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		res.render('itsd_status', {
			formObjects: formObjects
		});
	});
}



app.get('/finance_status', function (req, res) {
	if (req.session.FINANCE) {
		display_finance(res);
	}
});

function display_finance(res) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM historyform WHERE FORM_PCARD = ? ORDER BY EMP_LAST"
	var values = "on";
	connection.query(sql, [values], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		res.render('finance_status', {
			formObjects: formObjects
		});
	});
}







//login page function
//5.2 changed to res.render
app.get('/', function (req, res) {
	res.render('login');
});

//this post handles the checking of login credentials and sets the session variables.
app.post('/login_user', function (req, res, next) {
	var username = req.body.username;
	var password = req.body.password;
	var sql = "SELECT `username`, `REQUESTOR`, `FISC_ADMIN`, `DEPT_HEAD`, `BES`, `ITSD`, `FINANCE`, `FIRST_NAME`, `LAST_NAME` FROM `user_test` WHERE `username` = ? AND `password` = ?"
	console.log(req.body);
	connection.query(sql, [username, password], function (err, result, fields) {
		if (err) throw err;
		//if the input is incorrect redirect to login page.
		if (!result.length) {
			//res.redirect("/");
			//5.2
			res.render('login', { msg: "Invalid username or password." });

		} else {
			//if the user successfully logins in then set the session username and password.
			//set the session variables, the results are coming from the above query.
			req.session.REQUESTOR = result[0].REQUESTOR;
			req.session.FISC_ADMIN = result[0].FISC_ADMIN;
			req.session.DEPT_HEAD = result[0].DEPT_HEAD;
			req.session.BES = result[0].BES;
			req.session.ITSD = result[0].ITSD;
			req.session.FINANCE = result[0].FINANCE;

			req.session.FIRST_NAME = result[0].FIRST_NAME;
			req.session.LAST_NAME = result[0].LAST_NAME;

			req.session.username = username;
			req.session.password = password;
			//if they are an approve take them to prov_main, if not take them to the other views.
			//5.6
			if (req.session.REQUESTOR == "1" || req.session.FISC_ADMIN == "1" || req.session.DEPT_HEAD == "1") {
				res.redirect("/prov_main");
			} else if (req.session.BES == "1") {
				res.redirect("/bes_status");
			} else if (req.session.ITSD == "1") {
				res.redirect("/itsd_status");
			} else if (req.session.FINANCE) {
				res.redirect("/finance_status");
			}
		}
	});
});

//used to test if the session works. /admin should display your login name. it also allows you to logout
app.get('/admin', function (req, res) {

	if (req.session.username) {
		res.write('<h1>Hello ' + req.session.username + '</h1>');
		res.write('<a href="/logout">Logout</a>');
		res.end();
	} else {
		loginRedirect(res)
	}
});

//this is called when someone submits a form.
//the FORM variables = "on" if the masterform contains that subform
//e.g. when the for was submitted the "FORM_FUEL" and "FORM_PCARD" buttons were clicked. NULL is the default value.
//First we save the information from the form into variables. Then we insert that data into the database.
//We then check which forms are contained in the masterform by check the form variables for the value "on".
//Then we set the required approvals. e.g. REQUESTOR is required to approve a form so we set REQ_REQUESTOR = 1,
//when the REQ_REQUESTOR has approved it, it will change to "approved".

// FUNCTION ADDED FOR EMAIL USE
var api_key = '9879d49682d1f60c68ab1ba1f376b5c3-8b7bf2f1-5c36afbe';
var domain = 'sandboxb9e280a1cac14b909500c445f1224f24.mailgun.org';
var mailgun = require('mailgun-js')({ apiKey: api_key, domain: domain });

var validator = require('express-validator');
app.use(validator());
var validate = require('./validator.js');

app.post('/submit', function (req, res, next) {
	var FORM_FUEL = req.body.FORM_FUEL
	var FORM_PCARD = req.body.FORM_PCARD
	var FORM_IDCARD = req.body.FORM_IDCARD
	var FORM_HW_SW = req.body.FORM_HW_SW
	var FORM_USER_PROV = req.body.FORM_USER_PROV
	var FORM_SAP_ACCESS = req.body.FORM_SAP_ACCESS
	var FORM_CREATION_TIME = moment().format('MMMM Do YYYY, h:mm a');

	if (FORM_FUEL == "on") {
		submitForm(FORM_CREATION_TIME, FORM_FUEL, req, "FORM_FUEL");
	} if (FORM_PCARD == "on") {
		submitForm(FORM_CREATION_TIME, FORM_PCARD, req, "FORM_PCARD");
	} if (FORM_IDCARD == "on") {
		submitForm(FORM_CREATION_TIME, FORM_IDCARD, req, "FORM_IDCARD");
	} if (FORM_HW_SW == "on") {
		submitForm(FORM_CREATION_TIME, FORM_HW_SW, req, "FORM_HW_SW");
	} if (FORM_USER_PROV == "on") {
		submitForm(FORM_CREATION_TIME, FORM_USER_PROV, req, "FORM_USER_PROV");
	} if (FORM_SAP_ACCESS == "on") {
		submitForm(FORM_CREATION_TIME, FORM_SAP_ACCESS, req, "FORM_SAP_ACCESS");
	} res.redirect('/prov_main');
});


function submitForm(valTime, valEnding, req, ending) {
	var sql_1 = "INSERT INTO `masterform` SET`REQUESTOR`=?, `FISC_ADMIN`=?, `DEPT_HEAD`=?, `EMP_SUFFIX`=?, `EMP_FIRST`=?, `EMP_MI`=?, `EMP_LAST`=?, `EMP_SAP`=?, `EMP_USERNAME`=?, `EMP_EMAIL`=?\
		, `EMP_PHONE`=?, `EMP_DEPT`=?, `DEPT_PHONE_EXT`=?, `EMP_DIVISION`=?, `DEPT_COST_CENTER`=?, `EMP_JOB_STATUS`=?, `JOB_STATUS_OTHER`=?, `EMP_JOB_TITLE`=?\
		, `EMP_POSITION`=?, `MANAGER_NAME`=?, `MANAGER_PHONE`=?, `COMPANY_NAME`=?, `POC_NAME`=?, `POC_NUM`=?, `CONTRACT_NUM`=?, `CONTRACT_EXP_DATE`=?, `BESD_AD_VEHI_USE_BOOL`=?, `BESD_SAFETY_BOOL`=?, `PCARD_SELECT_TYPE`=?, `COSAID_FACI_ADDY`=?\
		, `COSAID_FACI_ENTRY`=?, `COSAID_FACI_NAME`=?, `COSAID_FACI_OTHER`=?, `COSAID_FACI_RM_ENTRY`=?, `COSAID_FACI_SUITE_ENTRY`=?\
		, `COSAID_REQUEST_TYPE_ID`=?, `COSAID_REQUEST_TYPE_FACACCESS`=?, `COSAID_REQUEST_TYPE_CJIS_FACACCESS`=?, `COSAID_REQUEST_TYPE_FACACCESS_CHANGE`=?\
		, `COSAID_REQUEST_TYPE_REISSUE`=?, `HWSW_REQUEST_HARDWARE`=?, `HWSW_REQUEST_SOFTWARE`=?, `HWSW_HW_COST_CENTER`=?, `HWSW_HW_GL_ACCT`=?\
		, `HWSW_SW_COST_CENTER`=?, `HWSW_SW_GL_ACCT`=?, `DEPT_BLDG_LOC`=?, `DEPT_BLDG_FLR`=?, `DEPT_BLDG_OFFC_NUM`=?, `HWSW_HW_REQUEST_TYPE`=?\
		, `HWSW_HW_CURRENT_DEVICE_ID`=?, `HWSW_HW_CHECKBOX_DESKTOP`=?, `HWSW_HW_CHECKBOX_LAPTOP`=?, `HWSW_HW_CHECKBOX_MONITOR`=?, `HWSW_HW_CHECKBOX_PRINTER`=?\
		, `HWSW_HW_CHECKBOX_DESKPHONE`=?, `HWSW_SW_NOT_STANDARD_BOOL`=?, `HWSW_SW_SELECT_ADOBEPRO`=?, `HWSW_SW_SELECT_DESC_ADOBEPRO`=?, `HWSW_SW_SELECT_QTY_ADOBEPRO`=?\
		, `HWSW_SW_SELECT_ADOBECREAT`=?, `HWSW_SW_SELECT_DESC_ADOBECREAT`=?, `HWSW_SW_SELECT_QTY_ADOBECREAT`=?, `HWSW_SW_SELECT_MSVISIO`=?\
		, `HWSW_SW_SELECT_QTY_MSVISIO`=?, `HWSW_SW_SELECT_MSPROJECT`=?, `HWSW_SW_SELECT_DESC_MSPROJECT`=?\
		, `HWSW_SW_SELECT_QTY_MSPROJECT`=?, `HWSW_SW_SELECT_ADOBEPS`=?, `HWSW_SW_SELECT_DESC_ADOBEPS`=?, `HWSW_SW_SELECT_QTY_ADOBEPS`=?\
		, `HWSW_SW_SELECT_EXCHANGEMAIL`=?, `HWSW_SW_SELECT_DESC_EXCHANGEMAIL`=?, `HWSW_SW_SELECT_QTY_EXCHANGEMAIL`=?, `HWSW_SW_SELECT_PREZIPRO`=?\
		, `HWSW_SW_SELECT_DESC_PREZIPRO`=?, `HWSW_SW_SELECT_QTY_PREZIPRO`=?, `HWSW_SW_SELECT_ADOBEACRO`=?, `HWSW_SW_SELECT_DESC_ADOBEACRO`=?\
		, `HWSW_SW_SELECT_QTY_ADOBEACRO`=?, `HWSW_SW_SELECT_ADOBEINDESIGN`=?, `HWSW_SW_SELECT_DESC_ADOBEINDESIGN`=?, `HWSW_SW_SELECT_QTY_ADOBEINDESIGN`=?\
		, `HWSW_SW_SELECT_VMWARE`=?, `HWSW_SW_SELECT_DESC_VMWARE`=?, `HWSW_SW_SELECT_QTY_VMWARE`=?, `PCARD_CH_LEGAL_NAME`=?, `PCARD_RECONC`=?, `PCARD_ACCT_COST_TYPE`=?\
		, `PCARD_ACCT_DEFAULT_CODE`=?, `PCARD_ACCT_DESC`=?, `PCARD_COMMENTS`=?, `PCARD_LIMITS_REQUESTED`=?, `PROV_ADDITIONAL_REQ`=?, `PROV_ADDITIONAL_REQ2`=?\
		, `PROV_ACCESS_REQUEST_HOME`=?, `PROV_DEVICE_ASSET_NUM`=?, `PROV_DEVICE_EXIST`=?, `PROV_ACCESS_REQUEST_SHARE`=?, `PROV_HIRE_DATE`=?, `PROV_ACCESS_REQUEST_EMAIL`=?\
		, `PROV_MAINFRAME_ACCESS`=?, `PROV_MAINFRAME_ACCESS2`=?, `PROV_PHONE_EXIST`=?, `PROV_PHONE_SET_ADMIN`=?, `PROV_PREV_NAME`=?, `PROV_PREV_SAP`=?, `PROV_USER_TYPE`=?\
		, `PROV_REQUEST_TYPE`=? , `PCARD_ROLE`=?, `PCARD_ROLE_DETAILS`=?, `PCARD_LIMITS_EXPLANATION`=?, `PCARD_RECONCILER_NAME`=?, `PCARD_RECONCILER_SAP`=? \
        , `PCARD_MAINT_CHANGE_IS`=?, `PCARD_MAINT_TEMP_DATE`=?, `PCARD_MAINT_NEW_USERNAME`=?, `PCARD_MAINT_FIRST`=?, `PCARD_MAINT_MI`=?, `PCARD_MAINT_LAST`=? \
        , `PCARD_MAINT_SUFFIX`=?, `PCARD_MAINT_ROLE_CHANGE`=?, `HWSW_HW_CHECKBOX_DESKTOP_DETAILS`=?, `HWSW_HW_CHECKBOX_LAPTOP_DETAILS`=?, `HWSW_HW_LAPTOP_EXTRA_1`=? \
        , `HWSW_HW_LAPTOP_EXTRA_2`=?, `HWSW_HW_LAPTOP_EXTRA_3`=?, `HWSW_HW_CHECKBOX_MONITOR_MODE`=?, `HWSW_HW_CHECKBOX_MONITOR_DETAILS`=? \
        , `HWSW_HW_CHECKBOX_IP_DESKPHONE_DETAILS`=?, `HWSW_HW_CHECKBOX_OTHER`=?, `PROV_PHONE_EXIST_EXT`=?, `PROV_PHONE_EXIST_MAC`=?, `PROV_PHONE_EXIST_SERIAL`=? \
        , `PROV_PHONE_SET_ADMIN_CONTACT`=?, `PROV_DISABLE_DATE`=?, `FORM_CREATION_TIME`=?, `CREATION_NAME`=?, `"
	var sql_2 = ending + "`=?"

	var values = [1, 1, 1, req.body.EMP_SUFFIX, req.body.EMP_FIRST, req.body.EMP_MI, req.body.EMP_LAST, req.body.EMP_SAP, req.body.EMP_USERNAME, req.body.EMP_EMAIL
		, req.body.EMP_PHONE, req.body.EMP_DEPT, req.body.DEPT_PHONE_EXT, req.body.EMP_DIVISION, req.body.DEPT_COST_CENTER, req.body.EMP_JOB_STATUS, req.body.JOB_STATUS_OTHER, req.body.EMP_JOB_TITLE
		, req.body.EMP_POSITION, req.body.MANAGER_NAME, req.body.MANAGER_PHONE, req.body.COMPANY_NAME, req.body.POC_NAME, req.body.POC_NUM, req.body.CONTRACT_NUM, req.body.CONTRACT_EXP_DATE, req.body.BESD_AD_VEHI_USE_BOOL
		, req.body.BESD_SAFETY_BOOL, req.body.PCARD_SELECT_TYPE, req.body.COSAID_FACI_ADDY, req.body.COSAID_FACI_ENTRY, req.body.COSAID_FACI_NAME, req.body.COSAID_FACI_OTHER
		, req.body.COSAID_FACI_RM_ENTRY, req.body.COSAID_FACI_SUITE_ENTRY, req.body.COSAID_REQUEST_TYPE_ID, req.body.COSAID_REQUEST_TYPE_FACACCESS, req.body.COSAID_REQUEST_TYPE_CJIS_FACACCESS
		, req.body.COSAID_REQUEST_TYPE_FACACCESS_CHANGE, req.body.COSAID_REQUEST_TYPE_REISSUE, req.body.HWSW_REQUEST_HARDWARE, req.body.HWSW_REQUEST_SOFTWARE, req.body.HWSW_HW_COST_CENTER, req.body.HWSW_HW_GL_ACCT
		, req.body.HWSW_SW_COST_CENTER, req.body.HWSW_SW_GL_ACCT, req.body.DEPT_BLDG_LOC, req.body.DEPT_BLDG_FLR, req.body.DEPT_BLDG_OFFC_NUM, req.body.HWSW_HW_REQUEST_TYPE
		, req.body.HWSW_HW_CURRENT_DEVICE_ID, req.body.HWSW_HW_CHECKBOX_DESKTOP, req.body.HWSW_HW_CHECKBOX_LAPTOP, req.body.HWSW_HW_CHECKBOX_MONITOR, req.body.HWSW_HW_CHECKBOX_PRINTER
		, req.body.HWSW_HW_CHECKBOX_DESKPHONE, req.body.HWSW_SW_NOT_STANDARD_BOOL, req.body.HWSW_SW_SELECT_ADOBEPRO, req.body.HWSW_SW_SELECT_DESC_ADOBEPRO, req.body.HWSW_SW_SELECT_QTY_ADOBEPRO
		, req.body.HWSW_SW_SELECT_ADOBECREAT, req.body.HWSW_SW_SELECT_DESC_ADOBECREAT, req.body.HWSW_SW_SELECT_QTY_ADOBECREAT, req.body.HWSW_SW_SELECT_MSVISIO
		, req.body.HWSW_SW_SELECT_QTY_MSVISIO, req.body.HWSW_SW_SELECT_MSPROJECT, req.body.HWSW_SW_SELECT_DESC_MSPROJECT
		, req.body.HWSW_SW_SELECT_QTY_MSPROJECT, req.body.HWSW_SW_SELECT_ADOBEPS, req.body.HWSW_SW_SELECT_DESC_ADOBEPS, req.body.HWSW_SW_SELECT_QTY_ADOBEPS
		, req.body.HWSW_SW_SELECT_EXCHANGEMAIL, req.body.HWSW_SW_SELECT_DESC_EXCHANGEMAIL, req.body.HWSW_SW_SELECT_QTY_EXCHANGEMAIL, req.body.HWSW_SW_SELECT_PREZIPRO
		, req.body.HWSW_SW_SELECT_DESC_PREZIPRO, req.body.HWSW_SW_SELECT_QTY_PREZIPRO, req.body.HWSW_SW_SELECT_ADOBEACRO, req.body.HWSW_SW_SELECT_DESC_ADOBEACRO
		, req.body.HWSW_SW_SELECT_QTY_ADOBEACRO, req.body.HWSW_SW_SELECT_ADOBEINDESIGN, req.body.HWSW_SW_SELECT_DESC_ADOBEINDESIGN, req.body.HWSW_SW_SELECT_QTY_ADOBEINDESIGN
		, req.body.HWSW_SW_SELECT_VMWARE, req.body.HWSW_SW_SELECT_DESC_VMWARE, req.body.HWSW_SW_SELECT_QTY_VMWARE, req.body.PCARD_CH_LEGAL_NAME, req.body.PCARD_RECONC, req.body.PCARD_ACCT_COST_TYPE
		, req.body.PCARD_ACCT_DEFAULT_CODE, req.body.PCARD_ACCT_DESC, req.body.PCARD_COMMENTS, req.body.PCARD_LIMITS_REQUESTED, req.body.PROV_ADDITIONAL_REQ, req.body.PROV_ADDITIONAL_REQ2
		, req.body.PROV_ACCESS_REQUEST_HOME, req.body.PROV_DEVICE_ASSET_NUM, req.body.PROV_DEVICE_EXIST, req.body.PROV_ACCESS_REQUEST_SHARE, req.body.PROV_HIRE_DATE, req.body.PROV_ACCESS_REQUEST_EMAIL
		, req.body.PROV_MAINFRAME_ACCESS, req.body.PROV_MAINFRAME_ACCESS2, req.body.PROV_PHONE_EXIST, req.body.PROV_PHONE_SET_ADMIN, req.body.PROV_PREV_NAME, req.body.PROV_PREV_SAP, req.body.PROV_USER_TYPE
		, req.body.PROV_REQUEST_TYPE, req.body.PCARD_ROLE, req.body.PCARD_ROLE_DETAILS, req.body.PCARD_LIMITS_EXPLANATION, req.body.PCARD_RECONCILER_NAME, req.body.PCARD_RECONCILER_SAP
		, req.body.PCARD_MAINT_CHANGE_IS, req.body.PCARD_MAINT_TEMP_DATE, req.body.PCARD_MAINT_NEW_USERNAME, req.body.PCARD_MAINT_FIRST, req.body.PCARD_MAINT_MI, req.body.PCARD_MAINT_LAST
		, req.body.PCARD_MAINT_SUFFIX, req.body.PCARD_MAINT_ROLE_CHANGE, req.body.HWSW_HW_CHECKBOX_DESKTOP_DETAILS, req.body.HWSW_HW_CHECKBOX_LAPTOP_DETAILS, req.body.HWSW_HW_LAPTOP_EXTRA_1
		, req.body.HWSW_HW_LAPTOP_EXTRA_2, req.body.HWSW_HW_LAPTOP_EXTRA_3, req.body.HWSW_HW_CHECKBOX_MONITOR_MODE, req.body.HWSW_HW_CHECKBOX_MONITOR_DETAILS
		, req.body.HWSW_HW_CHECKBOX_IP_DESKPHONE_DETAILS, req.body.HWSW_HW_CHECKBOX_OTHER, req.body.PROV_PHONE_EXIST_EXT, req.body.PROV_PHONE_EXIST_MAC, req.body.PROV_PHONE_EXIST_SERIAL
		, req.body.PROV_PHONE_SET_ADMIN_CONTACT, req.body.PROV_DISABLE_DATE,];
	//time creation name FORM_name
	var sql_3 = sql_1 + sql_2;
	values.push(valTime, req.session.FIRST_NAME, valEnding);

    validate.validate(req);
    
	connection.query(sql_3, values, function (err, result, fields) {
		if (err) throw err;
	});
	sql_3 = sql_3.slice(0, -1);
	values = values.slice(0, -2);
}


function display_bes_main(res) {
	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID \
			FROM masterform WHERE DEPT_HEAD = ? AND REQUESTOR = ? AND FISC_ADMIN = ? ORDER BY EMP_LAST"
	var values = ["1", "approved", "approved"];
	connection.query(sql, ["1", "approved", "approved"], function (err, result, fields) {
		var formObjects = result;
		////console.log(formObjects)
		res.render('prov_main', {
			formObjects: formObjects
		});
	});
}

app.post('/submit_prov_status', function (req, res, next) {
	var varApproved = "approved";
	var EMP_FIRST = req.body.EMP_FIRST;
	var EMP_LAST = req.body.EMP_LAST;
	var EMP_SAP = req.body.EMP_SAP;
	var PRIMARY_ID = req.body.PRIMARY_ID
	var inputValue = req.body.submit;

	//5.6
	if (req.session.BES == "1") {
		var sql = "SELECT * FROM historyform WHERE `PRIMARY_ID` = ?"
		connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
			if (err) throw err;
			var sessionId = "bes";
			var formObjects = result;
			res.render('view_approved_form_fulfillment', {
				formObjects: formObjects,
				sessionId: sessionId
			});
		});
	}
	if (req.session.ITSD == "1") {
		var sql = "SELECT * FROM historyform WHERE `PRIMARY_ID` = ?"
		connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
			if (err) throw err;
			var sessionId = "itsd";
			var formObjects = result;
			res.render('view_approved_form_fulfillment', {
				formObjects: formObjects,
				sessionId: sessionId
			});
		});
	}
	if (req.session.FINANCE == "1") {
		var sql = "SELECT * FROM historyform WHERE `PRIMARY_ID` = ?"
		connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
			if (err) throw err;
			var sessionId = "finance";
			var formObjects = result;
			res.render('view_approved_form_fulfillment', {
				formObjects: formObjects,
				sessionId: sessionId
			});
		});
	}

	var sql = "SELECT * FROM masterform WHERE `PRIMARY_ID` = ?"

	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		res.render('view_form', {
			formObjects: formObjects
		});
	});
});


app.post('/prov_status_archive', function (req, res, next) {
	var varApproved = "approved";
	var EMP_FIRST = req.body.EMP_FIRST;
	var EMP_LAST = req.body.EMP_LAST;
	var EMP_SAP = req.body.EMP_SAP;
	var PRIMARY_ID = req.body.PRIMARY_ID

	var sql = "SELECT * FROM historyform WHERE `PRIMARY_ID` = ?"

	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)
		res.render('archive_forms', {
			formObjects: formObjects
		});
	});

});


app.post('/prov_status_pending', function (req, res, next) {
	var varApproved = "approved";
	var EMP_FIRST = req.body.EMP_FIRST;
	var EMP_LAST = req.body.EMP_LAST;
	var EMP_SAP = req.body.EMP_SAP;
	var PRIMARY_ID = req.body.PRIMARY_ID

	var sql = "SELECT * FROM masterform WHERE `PRIMARY_ID` = ?"

	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)
		res.render('archive_forms', {
			formObjects: formObjects
		});
	});

});



app.post('/prov_status_rejected', function (req, res, next) {
	var varApproved = "approved";
	var EMP_FIRST = req.body.EMP_FIRST;
	var EMP_LAST = req.body.EMP_LAST;
	var EMP_SAP = req.body.EMP_SAP;
	var PRIMARY_ID = req.body.PRIMARY_ID

	var sql = "SELECT * FROM rejectedform WHERE `PRIMARY_ID` = ?"

	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		res.render('view_rejected_form', {
			formObjects: formObjects
		});
	});

});



app.get('/approval_status', function (req, res) {
	connection.query("SELECT EMP_FIRST, EMP_LAST, TEMP_ROLE_EXP, REQUESTOR FROM masterform ORDER BY EMP_LAST", function (err, result, fields) {
		var formObjects = result
		////console.log(formObjects)
		res.render('prov_status', {
			title: 'Express',
			formObjects: formObjects
		});
	});
});

//logout function
app.get('/logout', function (req, res) {

	// if the user logs out, destroy all of their individual session
	// information
	req.session.destroy(function (err) {
		if (err) {
			console.log(err);
		} else {
			res.redirect('/');
		}
	});

});
//5.2
function loginRedirect(res) {
	res.render('login', { msg: "Please login first." });
}

app.post('/submit_edit', function (req, res, next) {
	var varApproved = "approved";
	var EMP_FIRST = req.body.EMP_FIRST;
	var EMP_LAST = req.body.EMP_LAST;
	var EMP_SAP = req.body.EMP_SAP;
	var PRIMARY_ID = req.body.PRIMARY_ID;
	var inputValue = req.body.submit;
	console.log(inputValue);
	if (typeof req.body.submitValue !== 'undefined') {
		console.log("in submit" + req.body.submitValue)

		var sql_1 = "UPDATE `masterform` SET `EMP_SUFFIX`=?, `EMP_FIRST`=?, `EMP_MI`=?, `EMP_LAST`=?, `EMP_SAP`=?, `EMP_USERNAME`=?, `EMP_EMAIL`=?\
		, `EMP_PHONE`=?, `EMP_DEPT`=?, `DEPT_PHONE_EXT`=?, `EMP_DIVISION`=?, `DEPT_COST_CENTER`=?, `EMP_JOB_STATUS`=?, `JOB_STATUS_OTHER`=?, `EMP_JOB_TITLE`=?\
		, `EMP_POSITION`=?, `MANAGER_NAME`=?, `MANAGER_PHONE`=?, `COMPANY_NAME`=?, `POC_NAME`=?, `POC_NUM`=?, `CONTRACT_NUM`=?, `CONTRACT_EXP_DATE`=?, `BESD_AD_VEHI_USE_BOOL`=?, `BESD_SAFETY_BOOL`=?, `PCARD_SELECT_TYPE`=?, `COSAID_FACI_ADDY`=?\
		, `COSAID_FACI_ENTRY`=?, `COSAID_FACI_NAME`=?, `COSAID_FACI_OTHER`=?, `COSAID_FACI_RM_ENTRY`=?, `COSAID_FACI_SUITE_ENTRY`=?\
		, `COSAID_REQUEST_TYPE_ID`=?, `COSAID_REQUEST_TYPE_FACACCESS`=?, `COSAID_REQUEST_TYPE_CJIS_FACACCESS`=?, `COSAID_REQUEST_TYPE_FACACCESS_CHANGE`=?\
		, `COSAID_REQUEST_TYPE_REISSUE`=?, `HWSW_REQUEST_HARDWARE`=?, `HWSW_REQUEST_SOFTWARE`=?, `HWSW_HW_COST_CENTER`=?, `HWSW_HW_GL_ACCT`=?\
		, `HWSW_SW_COST_CENTER`=?, `HWSW_SW_GL_ACCT`=?, `DEPT_BLDG_LOC`=?, `DEPT_BLDG_FLR`=?, `DEPT_BLDG_OFFC_NUM`=?, `HWSW_HW_REQUEST_TYPE`=?\
		, `HWSW_HW_CURRENT_DEVICE_ID`=?, `HWSW_HW_CHECKBOX_DESKTOP`=?, `HWSW_HW_CHECKBOX_LAPTOP`=?, `HWSW_HW_CHECKBOX_MONITOR`=?, `HWSW_HW_CHECKBOX_PRINTER`=?\
		, `HWSW_HW_CHECKBOX_DESKPHONE`=?, `HWSW_SW_NOT_STANDARD_BOOL`=?, `HWSW_SW_SELECT_ADOBEPRO`=?, `HWSW_SW_SELECT_DESC_ADOBEPRO`=?, `HWSW_SW_SELECT_QTY_ADOBEPRO`=?\
		, `HWSW_SW_SELECT_ADOBECREAT`=?, `HWSW_SW_SELECT_DESC_ADOBECREAT`=?, `HWSW_SW_SELECT_QTY_ADOBECREAT`=?, `HWSW_SW_SELECT_MSVISIO`=?\
		, `HWSW_SW_SELECT_QTY_MSVISIO`=?, `HWSW_SW_SELECT_MSPROJECT`=?, `HWSW_SW_SELECT_DESC_MSPROJECT`=?\
		, `HWSW_SW_SELECT_QTY_MSPROJECT`=?, `HWSW_SW_SELECT_ADOBEPS`=?, `HWSW_SW_SELECT_DESC_ADOBEPS`=?, `HWSW_SW_SELECT_QTY_ADOBEPS`=?\
		, `HWSW_SW_SELECT_EXCHANGEMAIL`=?, `HWSW_SW_SELECT_DESC_EXCHANGEMAIL`=?, `HWSW_SW_SELECT_QTY_EXCHANGEMAIL`=?, `HWSW_SW_SELECT_PREZIPRO`=?\
		, `HWSW_SW_SELECT_DESC_PREZIPRO`=?, `HWSW_SW_SELECT_QTY_PREZIPRO`=?, `HWSW_SW_SELECT_ADOBEACRO`=?, `HWSW_SW_SELECT_DESC_ADOBEACRO`=?\
		, `HWSW_SW_SELECT_QTY_ADOBEACRO`=?, `HWSW_SW_SELECT_ADOBEINDESIGN`=?, `HWSW_SW_SELECT_DESC_ADOBEINDESIGN`=?, `HWSW_SW_SELECT_QTY_ADOBEINDESIGN`=?\
		, `HWSW_SW_SELECT_VMWARE`=?, `HWSW_SW_SELECT_DESC_VMWARE`=?, `HWSW_SW_SELECT_QTY_VMWARE`=?, `PCARD_CH_LEGAL_NAME`=?, `PCARD_RECONC`=?, `PCARD_ACCT_COST_TYPE`=?\
		, `PCARD_ACCT_DEFAULT_CODE`=?, `PCARD_ACCT_DESC`=?, `PCARD_COMMENTS`=?, `PCARD_LIMITS_REQUESTED`=?, `PROV_ADDITIONAL_REQ`=?, `PROV_ADDITIONAL_REQ2`=?\
		, `PROV_ACCESS_REQUEST_HOME`=?, `PROV_DEVICE_ASSET_NUM`=?, `PROV_DEVICE_EXIST`=?, `PROV_ACCESS_REQUEST_SHARE`=?, `PROV_HIRE_DATE`=?, `PROV_ACCESS_REQUEST_EMAIL`=?\
		, `PROV_MAINFRAME_ACCESS`=?, `PROV_MAINFRAME_ACCESS2`=?, `PROV_PHONE_EXIST`=?, `PROV_PHONE_SET_ADMIN`=?, `PROV_PREV_NAME`=?, `PROV_PREV_SAP`=?, `PROV_USER_TYPE`=?\
		, `PROV_REQUEST_TYPE`=? , `PCARD_ROLE`=?, `PCARD_ROLE_DETAILS`=?, `PCARD_LIMITS_EXPLANATION`=?, `PCARD_RECONCILER_NAME`=?, `PCARD_RECONCILER_SAP`=? \
        , `PCARD_MAINT_CHANGE_IS`=?, `PCARD_MAINT_TEMP_DATE`=?, `PCARD_MAINT_NEW_USERNAME`=?, `PCARD_MAINT_FIRST`=?, `PCARD_MAINT_MI`=?, `PCARD_MAINT_LAST`=? \
        , `PCARD_MAINT_SUFFIX`=?, `PCARD_MAINT_ROLE_CHANGE`=?, `HWSW_HW_CHECKBOX_DESKTOP_DETAILS`=?, `HWSW_HW_CHECKBOX_LAPTOP_DETAILS`=?, `HWSW_HW_LAPTOP_EXTRA_1`=? \
        , `HWSW_HW_LAPTOP_EXTRA_2`=?, `HWSW_HW_LAPTOP_EXTRA_3`=?, `HWSW_HW_CHECKBOX_MONITOR_MODE`=?, `HWSW_HW_CHECKBOX_MONITOR_DETAILS`=? \
        , `HWSW_HW_CHECKBOX_IP_DESKPHONE_DETAILS`=?, `HWSW_HW_CHECKBOX_OTHER`=?, `PROV_PHONE_EXIST_EXT`=?, `PROV_PHONE_EXIST_MAC`=?, `PROV_PHONE_EXIST_SERIAL`=? \
        , `PROV_PHONE_SET_ADMIN_CONTACT`=?, `PROV_DISABLE_DATE`=?, `PCARD_MAINT_LIMITS`=?, `PCARD_MAINT_NAME_CHANGE`=?, `PROV_DISABLE_DATE_LABEL`=?"
		var sql_3 = "WHERE `PRIMARY_ID` =?"

		var values_var = [req.body.EMP_SUFFIX, req.body.EMP_FIRST, req.body.EMP_MI, req.body.EMP_LAST, req.body.EMP_SAP, req.body.EMP_USERNAME, req.body.EMP_EMAIL
			, req.body.EMP_PHONE, req.body.EMP_DEPT, req.body.DEPT_PHONE_EXT, req.body.EMP_DIVISION, req.body.DEPT_COST_CENTER, req.body.EMP_JOB_STATUS, req.body.JOB_STATUS_OTHER, req.body.EMP_JOB_TITLE
			, req.body.EMP_POSITION, req.body.MANAGER_NAME, req.body.MANAGER_PHONE, req.body.COMPANY_NAME, req.body.POC_NAME, req.body.POC_NUM, req.body.CONTRACT_NUM, req.body.CONTRACT_EXP_DATE, req.body.BESD_AD_VEHI_USE_BOOL
			, req.body.BESD_SAFETY_BOOL, req.body.PCARD_SELECT_TYPE, req.body.COSAID_FACI_ADDY, req.body.COSAID_FACI_ENTRY, req.body.COSAID_FACI_NAME, req.body.COSAID_FACI_OTHER
			, req.body.COSAID_FACI_RM_ENTRY, req.body.COSAID_FACI_SUITE_ENTRY, req.body.COSAID_REQUEST_TYPE_ID, req.body.COSAID_REQUEST_TYPE_FACACCESS, req.body.COSAID_REQUEST_TYPE_CJIS_FACACCESS
			, req.body.COSAID_REQUEST_TYPE_FACACCESS_CHANGE, req.body.COSAID_REQUEST_TYPE_REISSUE, req.body.HWSW_REQUEST_HARDWARE, req.body.HWSW_REQUEST_SOFTWARE, req.body.HWSW_HW_COST_CENTER, req.body.HWSW_HW_GL_ACCT
			, req.body.HWSW_SW_COST_CENTER, req.body.HWSW_SW_GL_ACCT, req.body.DEPT_BLDG_LOC, req.body.DEPT_BLDG_FLR, req.body.DEPT_BLDG_OFFC_NUM, req.body.HWSW_HW_REQUEST_TYPE
			, req.body.HWSW_HW_CURRENT_DEVICE_ID, req.body.HWSW_HW_CHECKBOX_DESKTOP, req.body.HWSW_HW_CHECKBOX_LAPTOP, req.body.HWSW_HW_CHECKBOX_MONITOR, req.body.HWSW_HW_CHECKBOX_PRINTER
			, req.body.HWSW_HW_CHECKBOX_DESKPHONE, req.body.HWSW_SW_NOT_STANDARD_BOOL, req.body.HWSW_SW_SELECT_ADOBEPRO, req.body.HWSW_SW_SELECT_DESC_ADOBEPRO, req.body.HWSW_SW_SELECT_QTY_ADOBEPRO
			, req.body.HWSW_SW_SELECT_ADOBECREAT, req.body.HWSW_SW_SELECT_DESC_ADOBECREAT, req.body.HWSW_SW_SELECT_QTY_ADOBECREAT, req.body.HWSW_SW_SELECT_MSVISIO
			, req.body.HWSW_SW_SELECT_QTY_MSVISIO, req.body.HWSW_SW_SELECT_MSPROJECT, req.body.HWSW_SW_SELECT_DESC_MSPROJECT
			, req.body.HWSW_SW_SELECT_QTY_MSPROJECT, req.body.HWSW_SW_SELECT_ADOBEPS, req.body.HWSW_SW_SELECT_DESC_ADOBEPS, req.body.HWSW_SW_SELECT_QTY_ADOBEPS
			, req.body.HWSW_SW_SELECT_EXCHANGEMAIL, req.body.HWSW_SW_SELECT_DESC_EXCHANGEMAIL, req.body.HWSW_SW_SELECT_QTY_EXCHANGEMAIL, req.body.HWSW_SW_SELECT_PREZIPRO
			, req.body.HWSW_SW_SELECT_DESC_PREZIPRO, req.body.HWSW_SW_SELECT_QTY_PREZIPRO, req.body.HWSW_SW_SELECT_ADOBEACRO, req.body.HWSW_SW_SELECT_DESC_ADOBEACRO
			, req.body.HWSW_SW_SELECT_QTY_ADOBEACRO, req.body.HWSW_SW_SELECT_ADOBEINDESIGN, req.body.HWSW_SW_SELECT_DESC_ADOBEINDESIGN, req.body.HWSW_SW_SELECT_QTY_ADOBEINDESIGN
			, req.body.HWSW_SW_SELECT_VMWARE, req.body.HWSW_SW_SELECT_DESC_VMWARE, req.body.HWSW_SW_SELECT_QTY_VMWARE, req.body.PCARD_CH_LEGAL_NAME, req.body.PCARD_RECONC, req.body.PCARD_ACCT_COST_TYPE
			, req.body.PCARD_ACCT_DEFAULT_CODE, req.body.PCARD_ACCT_DESC, req.body.PCARD_COMMENTS, req.body.PCARD_LIMITS_REQUESTED, req.body.PROV_ADDITIONAL_REQ, req.body.PROV_ADDITIONAL_REQ2
			, req.body.PROV_ACCESS_REQUEST_HOME, req.body.PROV_DEVICE_ASSET_NUM, req.body.PROV_DEVICE_EXIST, req.body.PROV_ACCESS_REQUEST_SHARE, req.body.PROV_HIRE_DATE, req.body.PROV_ACCESS_REQUEST_EMAIL
			, req.body.PROV_MAINFRAME_ACCESS, req.body.PROV_MAINFRAME_ACCESS2, req.body.PROV_PHONE_EXIST, req.body.PROV_PHONE_SET_ADMIN, req.body.PROV_PREV_NAME, req.body.PROV_PREV_SAP, req.body.PROV_USER_TYPE
			, req.body.PROV_REQUEST_TYPE, req.body.PCARD_ROLE, req.body.PCARD_ROLE_DETAILS, req.body.PCARD_LIMITS_EXPLANATION, req.body.PCARD_RECONCILER_NAME, req.body.PCARD_RECONCILER_SAP
			, req.body.PCARD_MAINT_CHANGE_IS, req.body.PCARD_MAINT_TEMP_DATE, req.body.PCARD_MAINT_NEW_USERNAME, req.body.PCARD_MAINT_FIRST, req.body.PCARD_MAINT_MI, req.body.PCARD_MAINT_LAST
			, req.body.PCARD_MAINT_SUFFIX, req.body.PCARD_MAINT_ROLE_CHANGE, req.body.HWSW_HW_CHECKBOX_DESKTOP_DETAILS, req.body.HWSW_HW_CHECKBOX_LAPTOP_DETAILS, req.body.HWSW_HW_LAPTOP_EXTRA_1
			, req.body.HWSW_HW_LAPTOP_EXTRA_2, req.body.HWSW_HW_LAPTOP_EXTRA_3, req.body.HWSW_HW_CHECKBOX_MONITOR_MODE, req.body.HWSW_HW_CHECKBOX_MONITOR_DETAILS
			, req.body.HWSW_HW_CHECKBOX_IP_DESKPHONE_DETAILS, req.body.HWSW_HW_CHECKBOX_OTHER, req.body.PROV_PHONE_EXIST_EXT, req.body.PROV_PHONE_EXIST_MAC, req.body.PROV_PHONE_EXIST_SERIAL
			, req.body.PROV_PHONE_SET_ADMIN_CONTACT, req.body.PROV_DISABLE_DATE, req.body.PCARD_MAINT_LIMITS, req.body.PCARD_MAINT_NAME_CHANGE, req.body.PROV_DISABLE_DATE_LABEL,];

		//checking permissions 
		if (req.session.REQUESTOR == "1") {
			var REQUESTOR_TIME = moment().format('MMMM Do YYYY, h:mm a');
			var sql_2 = ", `REQUESTOR_TIME`=?,  `REQUESTOR`=?"
			//combine variables to form the sql call
			var sql = sql_1 + sql_2 + sql_3
			values_var.push(REQUESTOR_TIME, varApproved, PRIMARY_ID);

			connection.query(sql, values_var, function (err, result, fields) {
				if (err) throw err;
				//check to see if the form has been fully approved
				checkForms(PRIMARY_ID)
			});
			//remove the variables added
			sql = sql.slice(0, -2);
			values_var = values_var.slice(0, -3);

		} else if (req.session.FISC_ADMIN == "1") {
			var FISC_ADMIN_TIME = moment().format('MMMM Do YYYY, h:mm:ss a');
			var sql_2 = ", `FISC_ADMIN_TIME`=?, `FISC_ADMIN`=?"
			var sql = sql_1 + sql_2 + sql_3
			values_var.push(FISC_ADMIN_TIME, varApproved, PRIMARY_ID);

            var data = {
                from: 'Do Not Reply <autobeanteam@gmail.com>',
                to: 'autobeanteam@gmail.com',
                subject: 'Provision Form',
                text: 'Department Head, you have a new provisioning form for approval.\n\nFollow this link to view the form:    http://localhost:3306/'
            };

            // Use mailgun to send emails
            mailgun.messages().send(data, function (error, body) {
                if (error) {
                    console.log(error);
                }
                console.log(body);
            });
            
			connection.query(sql, values_var, function (err, result, fields) {
				if (err) throw err;
				checkForms(PRIMARY_ID)
			});
			sql = sql.slice(0, -2);
			values_var = values_var.slice(0, -3);

		} else if (req.session.DEPT_HEAD == "1") {
			var DEPT_HEAD_TIME = moment().format('MMMM Do YYYY, h:mm a');
			var sql_2 = ", `DEPT_HEAD_TIME` = ?, `DEPT_HEAD` = ?"
			var sql = sql_1 + sql_2 + sql_3
			values_var.push(DEPT_HEAD_TIME, varApproved, PRIMARY_ID);

			connection.query(sql, values_var, function (err, result, fields) {
				if (err) throw err;
				checkForms(PRIMARY_ID)
			});
			sql = sql.slice(0, -2);
			values_var = values_var.slice(0, -3);
		}
	} else {
		rejectForm(PRIMARY_ID, req.body.REJECTED_COMMENT);
	}
});


//checkForms checks if all approvals are "approved" 
//if they are it inserts the form into historyform and deletes it from masterform
function checkForms(PRIMARY_ID) {

	var sql = "SELECT * from masterform WHERE PRIMARY_ID = ?"
	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;

		if ((result[0].REQUESTOR == "approved" && result[0].FISC_ADMIN == "approved" && result[0].DEPT_HEAD == "approved")) {

			var sql = "INSERT INTO historyform SELECT * from masterform WHERE PRIMARY_ID = ?"
			connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
				if (err) throw err;

				var sql = "DELETE FROM masterform WHERE PRIMARY_ID = ?"
				connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
					if (err) throw err;
					console.log("Number of records deleted: " + result.affectedRows);
				});
			});
		}
	});
}


//5.2
//rejection
function rejectForm(PRIMARY_ID, txtResponse) {
	var sql = "SELECT * from masterform WHERE PRIMARY_ID = ?"
	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;


		var sql = "INSERT INTO rejectedform SELECT * from masterform WHERE PRIMARY_ID = ?"
		connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
			if (err) throw err;

			var sql = "UPDATE rejectedform SET REJECTED_COMMENT = ? WHERE PRIMARY_ID = ?"
			connection.query(sql, [txtResponse, PRIMARY_ID], function (err, result, fields) {
				if (err) throw err;


				var sql = "DELETE FROM masterform WHERE PRIMARY_ID = ?"
				connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
					if (err) throw err;
					console.log("Number of records deleted: " + result.affectedRows);
				});
			});
		});
	});
}
//new 5.2
//query historyform to find information
//redirect to prov_add.ejs
//fill in the fields from the query
app.post('/autofill_prov_add', function (req, res) {
	var sql = "SELECT `EMP_SAP`, `EMP_FIRST`, `EMP_LAST`, `EMP_SUFFIX`, `EMP_MI` FROM historyform WHERE EMP_SAP = ? LIMIT 1"
	connection.query(sql, [req.body.EMP_SAP], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		if (result.length > 0) {
			res.render('prov_add', {
				formObjects: formObjects
			});
		} else {
			res.render('prov_add', { VAR_EMP_SAP: req.body.EMP_SAP });
		}
	});
});



//5.6
app.get('/user_active_forms', function (req, res) {

	var sql = "SELECT EMP_FIRST, EMP_LAST, EMP_SAP, REQUESTOR, REQUESTOR_TIME, FISC_ADMIN, FISC_ADMIN_TIME, DEPT_HEAD, DEPT_HEAD_TIME, BES, ITSD, FINANCE,\
			FORM_FUEL, FORM_PCARD, FORM_IDCARD, FORM_HW_SW, FORM_USER_PROV, FORM_SAP_ACCESS, PRIMARY_ID FROM masterform WHERE CREATION_NAME = ? ORDER BY EMP_LAST"
	var values = req.session.FIRST_NAME;

	connection.query(sql, [values], function (err, result, fields) {
		var formObjects = result;
		////console.log(formObjects)
		res.render('user_active_forms', {
			formObjects: formObjects
		});
	});

});


//5.6
app.post('/active_status', function (req, res, next) {
	var PRIMARY_ID = req.body.PRIMARY_ID

	var sql = "SELECT * FROM masterform WHERE `PRIMARY_ID` = ?"

	connection.query(sql, [PRIMARY_ID], function (err, result, fields) {
		if (err) throw err;
		var formObjects = result;
		////console.log(formObjects)
		res.render('view_form', {
			formObjects: formObjects
		});
	});
});