//fades div# into or out of view when btn# is pressed.
$(document).ready(function () {

    //Multiple Selector - Clicks for buttons 1-6
    $("#btn1,#btn2,#btn3,#btn4,#btn5,#btn6").click(function () {
        // Each click checks for different combinations of check detection
        // Click ID 3a
        if ($("#btn2").is(':checked') || $("#btn4").is(':checked') || $("#btn6").is(':checked')) {
            $("#EMP_USERNAME").show();
        } else if ($("#btn2").not(':checked') || $("#btn4").not(':checked') || $("#btn6").not(':checked')) {
            $("#EMP_USERNAME").hide();
        }
        // Click ID 3b
        if ($("#btn1").is(':checked') || $("#btn4").is(':checked') || $("#btn5").is(':checked')) {
            $("#DEPT_PHONE_EXT").show();
        } else if ($("#btn1").not(':checked') || $("#btn4").not(':checked') || $("#btn5").not(':checked')) {
            $("#DEPT_PHONE_EXT").hide();
        }
        // Click ID 2a
        if ($("#btn2").is(':checked') || $("#btn6").is(':checked')) {
            $("#EMP_EMAIL").show();
        } else if ($("#btn2").not(':checked') || $("#btn6").not(':checked')) {
            $("#EMP_EMAIL").hide();
        }
        // Click ID 2b
        if ($("#btn1").is(':checked') || $("#btn3").is(':checked')) {
            $("#EMP_PHONE,#EMP_DIVISION").show();
        } else if ($("#btn1").not(':checked') || $("#btn3").not(':checked')) {
            $("#EMP_PHONE,#EMP_DIVISION").hide();
        }

        // Click ID 2c
        if ($("#btn4").is(':checked') || $("#btn6").is(':checked')) {
        } else if ($("#btn4").not(':checked') || $("#btn6").not(':checked')) {
            $("#DEPT_BLDG_LOC,#DEPT_BLDG_FLR,#DEPT_BLDG_OFFC_NUM").hide();
        }

        // Click ID 2d
        if ($("#btn3").is(':checked') || $("#btn6").is(':checked')) {
            $("#MANAGER_NAME").show();
        } else if ($("#btn3").not(':checked') || $("#btn6").not(':checked')) {
            $("#MANAGER_NAME").hide();
        }

        // Click ID 1a
        if ($("#btn1").is(':checked')) {
            $("#label_1").show();
            $("#BESD_COST_CENTER").show();
            $("#BESD_AD_VEHI_USE_BOOL,#BESD_AD_VEHI_USE_BOOL2").show();
            $("#BESD_SAFETY_BOOL,#BESD_SAFETY_BOOL2").show();
            $('#BESD_AD_VEHI_USE_BOOL,#BESD_SAFETY_BOOL').prop('required', true);
        } else if ($("#btn1").not(':checked')) {
            $("#BESD_COST_CENTER").hide();
            $("#label_1").hide();
            $("#BESD_AD_VEHI_USE_BOOL,#BESD_AD_VEHI_USE_BOOL2").hide();
            $("#BESD_SAFETY_BOOL,#BESD_SAFETY_BOOL2").hide();
            $('#BESD_AD_VEHI_USE_BOOL,#BESD_SAFETY_BOOL').removeAttr('required');
        }
        // Click ID 1b
        if ($("#btn2").is(':checked')) {
            $("#label_2,#PCARD_SELECT_TYPE,#PCARD_COMMENTS").show();
        } else if ($("#btn2").not(':checked')) {
            $("#label_2,#label_2b").hide();
            $("#PCARD_RECONCILER_NAME,#PCARD_RECONCILER_SAP").hide();
            $("#PCARD_SELECT_TYPE,#PCARD_ROLE").hide();
            $('#PCARD_SELECT_TYPE,#PCARD_ROLE').prop('selectedIndex', 0); // resets PCARD_SELECT_TYPE, PCARD_ROLE
            $("#PCARD_RECONC").prop("checked", false);
            $("#PCARD_ACCT_COST_TYPE,#PCARD_ACCT_DEFAULT_CODE,#PCARD_ACCT_DESC").hide();
            $("#PCARD_CH_LEGAL_NAME,#PCARD_COMMENTS,#PCARD_LIMITS_REQUESTED").hide();

            $("#label_0,#COMPANY_NAME,#POC_NAME,#POC_NUM,#CONTRACT_NUM").hide();
            $("#CONTRACT_EXP_DATE,#CONTRACT_EXP_DATE_LABEL").hide();
            $("#JOB_STATUS_OTHER").hide();

            $("#PCARD_MAINT_LAST_FOUR").hide();
            $("#PCARD_MAINT_CHANGE_IS").hide();
            $('#PCARD_MAINT_CHANGE_IS').prop('selectedIndex', 0);
            $("#PCARD_MAINT_TEMP_DATE,#PCARD_MAINT_TEMP_DATE_LABEL").hide();
            $("#PCARD_MAINT_LIMITS_LABEL").hide();
            $("#PCARD_MAINT_LIMITS").prop("checked", false);

            $("#PCARD_ROLE_DETAILS").hide();
            $("#PCARD_LIMITS_EXPLANATION").hide();
            $('#PCARD_ACCT_COST_TYPE,#PCARD_LIMITS_REQUESTED').prop('selectedIndex', 0);

            $("#PCARD_MAINT_NEW_USERNAME,#PCARD_MAINT_NAME_CHANGE_FIELDS").hide();

            $("#PCARD_MAINT_ROLE_CHANGE").prop("checked", false);
            $("#PCARD_MAINT_ROLE_CHANGE_LABEL").hide();

            $("#PCARD_MAINT_NAME_CHANGE").prop("checked", false);
            $("#PCARD_MAINT_NAME_CHANGE_LABEL").hide();
        }
        // Click ID 1c
        if ($("#btn3").is(':checked')) {
            $("#label_3,#label_3b").show();
            $("#MANAGER_PHONE").show();
            $("#EMP_JOB_STATUS,#EMP_JOB_TITLE").show();
            $("#COSAID_REQUEST_TYPE_1,#COSAID_REQUEST_TYPE_2,#COSAID_REQUEST_TYPE_3").show();
            $("#COSAID_REQUEST_TYPE_4,#COSAID_REQUEST_TYPE_5").show();
        } else if ($("#btn3").not(':checked')) {
            $("#label_3,#label_3b").hide();
            $("#MANAGER_PHONE").hide();
            $("#label_3c,#EMP_JOB_STATUS,#EMP_JOB_TITLE").hide();
            $('#EMP_JOB_STATUS').prop('selectedIndex', 0);
            $("#COSAID_REQUEST_TYPE_1,#COSAID_REQUEST_TYPE_2,#COSAID_REQUEST_TYPE_3").hide();
            $("#COSAID_REQUEST_TYPE_4,#COSAID_REQUEST_TYPE_5").hide();
            $("#COSAID_FACI_NAME,#COSAID_FACI_ADDY,#COSAID_FACI_ENTRY").hide();
            $("#COSAID_FACI_SUITE_ENTRY,#COSAID_FACI_RM_ENTRY,#COSAID_FACI_OTHER").hide();
            $("#COSAID_REQUEST_TYPE_FACACCESS,#COSAID_REQUEST_TYPE_CJIS_FACACCESS,#COSAID_REQUEST_TYPE_FACACCESS_CHANGE").prop("checked", false);
        }
        // Click ID 1d
        if ($("#btn4").is(':checked')) {
            $("#label_4").show();
            $("#HWSW_REQUEST_1,#HWSW_REQUEST_2").show();
        } else if ($("#btn4").not(':checked')) {
            $("#label_4").hide();
            $("#HWSW_REQUEST_1,#HWSW_REQUEST_2").hide();

            // This can probably be modularized
            // Uncheck and rehide hardware options
            $("#HWSW_REQUEST_HARDWARE").prop("checked", false);
            $("#EMP_POSITION").hide();
            $("#label_4b").hide();
            $("#DEPT_BLDG_FLR,#DEPT_BLDG_LOC,#DEPT_BLDG_OFFC_NUM").hide();
            $("#HWSW_HW_COST_CENTER,#HWSW_HW_GL_ACCT").hide();
            $("#HWSW_HW_REQUEST_TYPE,#HWSW_HW_CURRENT_DEVICE_ID").hide();
            $("#HWSW_HW_CHECKBOX_1,#HWSW_HW_CHECKBOX_2,#HWSW_HW_CHECKBOX_3").hide();
            $("#HWSW_HW_CHECKBOX_4,#HWSW_HW_CHECKBOX_5").hide();

            // This can probably me modularized
            // Uncheck and rehide software options
            $("#HWSW_REQUEST_SOFTWARE").prop("checked", false);
            $("#label_4c").hide();
            $("#HWSW_SW_COST_CENTER,#HWSW_SW_GL_ACCT").hide();
            $("#HWSW_SW_NOT_STANDARD_BOOL").hide();
            $("#HWSW_SW_NOT_STANDARD_BOOL").prop("checked", false);
            $("#HWSW_SW_NOT_STANDARD_BOOL_LABEL,#HWSW_SW_NOT_STANDARD_BOOL_LABEL2").hide();
            $("#HWSW_SW_OTHER_IMAGE").hide();

            // This can probably be modularized
            // Rehide not standard software
            $("#HWSW_SW_SELECT_1,#HWSW_SW_OTHER_DESC_1,#HWSW_SW_SELECT_QTY_ADOBEPRO").hide();
            $("#HWSW_SW_SELECT_2,#HWSW_SW_OTHER_DESC_2,#HWSW_SW_SELECT_QTY_ADOBECREAT").hide();
            $("#HWSW_SW_SELECT_3,#HWSW_SW_OTHER_DESC_3,#HWSW_SW_SELECT_QTY_MSVISIO").hide();
            $("#HWSW_SW_SELECT_4,#HWSW_SW_OTHER_DESC_4,#HWSW_SW_SELECT_QTY_MSPROJECT").hide();
            $("#HWSW_SW_SELECT_5,#HWSW_SW_OTHER_DESC_5,#HWSW_SW_SELECT_QTY_ADOBEPS").hide();
            $("#HWSW_SW_SELECT_6,#HWSW_SW_OTHER_DESC_6,#HWSW_SW_SELECT_QTY_EXCHANGEMAIL").hide();
            $("#HWSW_SW_SELECT_7,#HWSW_SW_OTHER_DESC_7,#HWSW_SW_SELECT_QTY_PREZIPRO").hide();
            $("#HWSW_SW_SELECT_8,#HWSW_SW_OTHER_DESC_8,#HWSW_SW_SELECT_QTY_ADOBEACRO").hide();
            $("#HWSW_SW_SELECT_9,#HWSW_SW_OTHER_DESC_9,#HWSW_SW_SELECT_QTY_ADOBEINDESIGN").hide();
            $("#HWSW_SW_SELECT_10,#HWSW_SW_OTHER_DESC_10,#HWSW_SW_SELECT_QTY_VMWARE").hide();
        }
        // Click ID 1e
        if ($("#btn5").is(':checked')) {
            $("#label_5,#PROV_REQUEST_TYPE").show();
        } else if ($("#btn5").not(':checked')) {
            $("#label_5").hide();
            $("#label_5b,#label_5c,#label_5d,#label_5e").hide(); // User Access, Access Requested, Additional Req, Mainframe
            $("#PROV_REQUEST_TYPE").hide();
            $('#PROV_REQUEST_TYPE').prop('selectedIndex', 0); // reset prov request type

            $("#PROV_USER_TYPE,#PROV_HIRE_DATE_LABEL,#PROV_DEVICE_EXIST_BOOL_LABEL").hide();
            $('#PROV_USER_TYPE').prop('selectedIndex', 0); // reset prov user type
            $("#PROV_DEVICE_ASSET_NUM").hide();
            $("#PROV_DEVICE_EXIST,#PROV_PHONE_EXIST,#PROV_PHONE_SET_ADMIN").prop("checked", false);
            $("#PROV_PHONE_EXIST_BOOL_LABEL,#PROV_PHONE_SET_ADMIN_BOOL_LABEL").hide();
            $("#PROV_PHONE_EXIST_FIELDS,#PROV_PHONE_SET_ADMIN_CONTACT").hide();

            $("#PROV_ACCESS_REQUEST_1,#PROV_ACCESS_REQUEST_2,#PROV_ACCESS_REQUEST_3").hide(); // labels for (H) (K) Email
            $("#PROV_ACCESS_REQUEST_HOME,#PROV_ACCESS_REQUEST_SHARE,#PROV_ACCESS_REQUEST_EMAIL").prop("checked", false);
            $("#PROV_PREV_NAME,#PROV_PREV_SAP").hide();

            $("#PROV_ADDITIONAL_REQ,#PROV_MAINFRAME_ACCESS").hide();
            $("#PROV_ADDITIONAL_REQ,#PROV_MAINFRAME_ACCESS").prop("checked", false);
            $("#PROV_ADDITIONAL_REQ2,#PROV_MAINFRAME_ACCESS2").hide();

            $("#PROV_DISABLE_DATE_LABEL").hide();
        }

        // Click ID 1f
        if ($("#btn6").is(':checked')) {
            $("#label_6").show();
            $("#ID_EXP_DATE").show();
            $("#TEMP_BOOL,#TEMP_BOOL2").show();
            $("#TEMP_ROLE_BOOL,#TEMP_ROLE_BOOL2").show();
            $("#TEMP_ROLE_EXP,#WORK_LOC").show();
        } else if ($("#btn6").not(':checked')) {
            $("#label_6").hide();
            $("#ID_EXP_DATE").hide();
            $("#TEMP_BOOL,#TEMP_BOOL2").hide();
            $("#TEMP_ROLE_BOOL,#TEMP_ROLE_BOOL2").hide();
            $("#TEMP_ROLE_EXP,#WORK_LOC").hide();
        }
    });

    /* Click ID Template START
        if ( $("#btn").is(':checked')) {
            $("#").show();
        } else if ( $("#btn").not(':checked')){
            $("#").hide();
        }
    Click ID Template END */

    // SUB-BUTTONS

    // PCARDs
    $("#PCARD_SELECT_TYPE").change(function () {
        if ($("#PCARD_SELECT_TYPE").val() == 'CEO Website Access Only') {
            // show
            $("#PCARD_ROLE").show();

            // hide
            $("#PCARD_ROLE_DETAILS").hide();
            $('#PCARD_ROLE').prop('selectedIndex', 0); // resets PCARD_ROLE

            // new_cc
            $("#label_2b").hide();
            $("#PCARD_RECONCILER_NAME,#PCARD_RECONCILER_SAP").hide();
            $("#PCARD_ACCT_COST_TYPE,#PCARD_ACCT_DEFAULT_CODE,#PCARD_ACCT_DESC").hide();
            $("#PCARD_CH_LEGAL_NAME,#PCARD_LIMITS_REQUESTED").hide();
            $("#PCARD_RECONC,#PCARD_MAINT_ROLE_CHANGE").prop("checked", false);
            $("#PCARD_LIMITS_EXPLANATION").hide();
            $('#PCARD_ACCT_COST_TYPE,#PCARD_LIMITS_REQUESTED').prop('selectedIndex', 0);

            // maintenance
            $("#PCARD_MAINT_CHANGE_IS").hide();
            $('#PCARD_MAINT_CHANGE_IS').prop('selectedIndex', 0);
            $("#PCARD_MAINT_TEMP_DATE,#PCARD_MAINT_TEMP_DATE_LABEL").hide();
            $("#PCARD_MAINT_LIMITS_LABEL").hide();
            $("#PCARD_MAINT_LIMITS").prop("checked", false);
            $("#PCARD_MAINT_ROLE_CHANGE").prop("checked", false);
            $("#PCARD_MAINT_ROLE_CHANGE_LABEL").hide();
            $("#PCARD_MAINT_NAME_CHANGE").prop("checked", false);
            $("#PCARD_MAINT_NAME_CHANGE_LABEL,#PCARD_MAINT_NEW_USERNAME,#PCARD_MAINT_NAME_CHANGE_FIELDS").hide();
            $("#PCARD_MAINT_LAST_FOUR").hide();
        } else if ($("#PCARD_SELECT_TYPE").val() == 'New Credit Card Request') {
            // show
            $("#label_2b").show();
            $("#PCARD_ACCT_COST_TYPE,#PCARD_ACCT_DEFAULT_CODE,#PCARD_ACCT_DESC").show();
            $("#PCARD_CH_LEGAL_NAME,#PCARD_COMMENTS,#PCARD_LIMITS_REQUESTED").show();
            // hide
            $("#PCARD_ROLE").hide();
            $('#PCARD_ROLE,#PCARD_LIMITS_REQUESTED').prop('selectedIndex', 0); // resets PCARD_ROLE
            $("#PCARD_ROLE_DETAILS,#PCARD_LIMITS_EXPLANATION").hide();

            $("#PCARD_MAINT_ROLE_CHANGE").prop("checked", false);
            $("#PCARD_MAINT_ROLE_CHANGE_LABEL").hide();

            $("#PCARD_MAINT_CHANGE_IS").hide();
            $('#PCARD_MAINT_CHANGE_IS').prop('selectedIndex', 0);
            $("#PCARD_MAINT_TEMP_DATE,#PCARD_MAINT_TEMP_DATE_LABEL").hide();
            $("#PCARD_MAINT_LIMITS_LABEL").hide();
            $("#PCARD_MAINT_LIMITS").prop("checked", false);
            $("#PCARD_MAINT_NAME_CHANGE").prop("checked", false);
            $("#PCARD_MAINT_NAME_CHANGE_LABEL,#PCARD_MAINT_NEW_USERNAME,#PCARD_MAINT_NAME_CHANGE_FIELDS").hide();
            $("#PCARD_MAINT_LAST_FOUR").hide();
        } else if ($("#PCARD_SELECT_TYPE").val() == 'Cardholder Maintenance') {
            //show
            $("#PCARD_MAINT_LAST_FOUR").show();
            $("#PCARD_MAINT_CHANGE_IS,#PCARD_MAINT_ROLE_CHANGE_LABEL").show();
            $("#PCARD_MAINT_LIMITS_LABEL").show();
            $("#PCARD_MAINT_NAME_CHANGE_LABEL").show();
            //hide
            $("#label_2b").hide();
            $("#PCARD_RECONCILER_NAME,#PCARD_RECONCILER_SAP").hide();
            $("#PCARD_ACCT_COST_TYPE,#PCARD_ACCT_DEFAULT_CODE,#PCARD_ACCT_DESC").hide();
            $("#PCARD_CH_LEGAL_NAME,#PCARD_LIMITS_REQUESTED").hide();
            $("#PCARD_RECONC,#PCARD_MAINT_ROLE_CHANGE").prop("checked", false);
            $("#PCARD_LIMITS_EXPLANATION").hide();
            $('#PCARD_ACCT_COST_TYPE,#PCARD_LIMITS_REQUESTED').prop('selectedIndex', 0);

            $("#PCARD_ROLE").hide();
            $('#PCARD_ROLE').prop('selectedIndex', 0); // resets PCARD_SELECT_TYPE, PCARD_ROLE
            $("#PCARD_ROLE_DETAILS").hide();
        } else {
            $("#label_2b").hide();
            $("#PCARD_RECONCILER_NAME,#PCARD_RECONCILER_SAP").hide();
            $("#PCARD_ACCT_COST_TYPE,#PCARD_ACCT_DEFAULT_CODE,#PCARD_ACCT_DESC").hide();
            $("#PCARD_CH_LEGAL_NAME,#PCARD_COMMENTS,#PCARD_LIMITS_REQUESTED").hide();
            $("#PCARD_LIMITS_EXPLANATION").hide();
            $("#PCARD_ROLE").hide();
            $('#PCARD_ROLE,#PCARD_ACCT_COST_TYPE,#PCARD_LIMITS_REQUESTED').prop('selectedIndex', 0);
            $("#PCARD_RECONC").prop("checked", false);

            $("#PCARD_MAINT_CHANGE_IS").hide();
            $("#PCARD_MAINT_TEMP_DATE,#PCARD_MAINT_TEMP_DATE_LABEL").hide();
            $("#PCARD_MAINT_LIMITS_LABEL").hide();
            $("#PCARD_MAINT_LIMITS").prop("checked", false);
            $("#PCARD_MAINT_LAST_FOUR").hide();
        }
    });

    // PCard: Maintenance - Change Type
    $("#PCARD_MAINT_CHANGE_IS").change(function () {
        if ($("#PCARD_MAINT_CHANGE_IS").val() == 'TEMP') {
            $("#PCARD_MAINT_TEMP_DATE,#PCARD_MAINT_TEMP_DATE_LABEL").show();
        } else {
            $("#PCARD_MAINT_TEMP_DATE,#PCARD_MAINT_TEMP_DATE_LABEL").hide();
        }
    });

    // PCard: Maintenance - Change Profile Transaction Limits
    $("#PCARD_MAINT_LIMITS").click(function () {
        if ($("#PCARD_MAINT_LIMITS").is(':checked')) {
            $("#PCARD_LIMITS_REQUESTED").show();
        } else {
            $("#PCARD_LIMITS_REQUESTED").hide();
            $('#PCARD_LIMITS_REQUESTED').prop('selectedIndex', 0);
        }
    });

    $("#PCARD_ROLE").change(function () {
        if ($("#PCARD_ROLE").val() == 'Approver' || $("#PCARD_ROLE").val() == 'Reconciler') {
            // show
            $("#PCARD_ROLE_DETAILS").show();
        } else {
            $("#PCARD_ROLE_DETAILS").hide();
        }
    });

    $("#PCARD_RECONC").click(function () {
        if ($("#PCARD_RECONC").is(':checked')) {
            $("#PCARD_RECONCILER_NAME,#PCARD_RECONCILER_SAP").show();
        } else {
            $("#PCARD_RECONCILER_NAME,#PCARD_RECONCILER_SAP").hide();
        }
    });

    $("#PCARD_LIMITS_REQUESTED").change(function () {
        if ($("#PCARD_LIMITS_REQUESTED").val() == 'OTHER') {
            // show
            $("#PCARD_LIMITS_EXPLANATION").show();
        } else {
            $("#PCARD_LIMITS_EXPLANATION").hide();
        }
    });

    // PCard - Cardholder or username change
    $("#PCARD_MAINT_NAME_CHANGE").click(function () {
        if ($(this).is(':checked')) {
            $("#PCARD_MAINT_NEW_USERNAME,#PCARD_MAINT_NAME_CHANGE_FIELDS").show();
        } else if ($(this).not(':checked')) {
            $("#PCARD_MAINT_NEW_USERNAME,#PCARD_MAINT_NAME_CHANGE_FIELDS").hide();
        }
    });

    // PCard - CEO Role Change
    $("#PCARD_MAINT_ROLE_CHANGE").click(function () {
        if ($(this).is(':checked')) {
            $("#PCARD_ROLE").show();
        } else if ($(this).not(':checked') && $("#PCARD_SELECT_TYPE").val() != 'CEO Website Access Only') {
            $("#PCARD_ROLE").hide();
        }
    });

    // ID CARD - Contractor/Vendor Info
    $("#EMP_JOB_STATUS").change(function () {
        console.log($("#EMP_JOB_STATUS").val());
        if ($("#EMP_JOB_STATUS").val() == 'VENDOR' || $("#EMP_JOB_STATUS").val() == 'CONTRACTOR' || $("#EMP_JOB_STATUS").val() == 'CONTRACT_TEMP') {
            // show
            $("#label_0,#COMPANY_NAME,#POC_NAME,#POC_NUM,#CONTRACT_NUM").show();
            $("#CONTRACT_EXP_DATE,#CONTRACT_EXP_DATE_LABEL").show();
            // hide
            $("#JOB_STATUS_OTHER").hide();
        } else if ($("#EMP_JOB_STATUS").val() == 'OTHER') {
            // show
            $("#JOB_STATUS_OTHER").show();
            // hide
            $("#label_0,#COMPANY_NAME,#POC_NAME,#POC_NUM,#CONTRACT_NUM").hide();
            $("#CONTRACT_EXP_DATE,#CONTRACT_EXP_DATE_LABEL").hide();
        } else {
            $("#label_0,#COMPANY_NAME,#POC_NAME,#POC_NUM,#CONTRACT_NUM").hide();
            $("#CONTRACT_EXP_DATE,#CONTRACT_EXP_DATE_LABEL").hide();
            $("#JOB_STATUS_OTHER").hide();
        }
    });

    // CoSAID Facility
    $("#COSAID_REQUEST_TYPE_FACACCESS,#COSAID_REQUEST_TYPE_CJIS_FACACCESS,#COSAID_REQUEST_TYPE_FACACCESS_CHANGE").click(function () {
        if ($("#COSAID_REQUEST_TYPE_FACACCESS").is(':checked') || $("#COSAID_REQUEST_TYPE_CJIS_FACACCESS").is(':checked') || $("#COSAID_REQUEST_TYPE_FACACCESS_CHANGE").is(':checked')) {
            $("#label_3c,#COSAID_FACI_NAME,#COSAID_FACI_ADDY,#COSAID_FACI_ENTRY").show();
            $("#COSAID_FACI_SUITE_ENTRY,#COSAID_FACI_RM_ENTRY,#COSAID_FACI_OTHER").show();
        } else if ($("#COSAID_REQUEST_TYPE_FACACCESS").not(':checked') || $("#COSAID_REQUEST_TYPE_CJIS_FACACCESS").not(':checked') || $("#COSAID_REQUEST_TYPE_FACACCESS_CHANGE").not(':checked')) {
            $("#label_3c,#COSAID_FACI_NAME,#COSAID_FACI_ADDY,#COSAID_FACI_ENTRY").hide();
            $("#COSAID_FACI_SUITE_ENTRY,#COSAID_FACI_RM_ENTRY,#COSAID_FACI_OTHER").hide();
        }
    });

    // Provisioning Hardware
    $("#HWSW_REQUEST_HARDWARE").click(function () {
        if ($(this).is(':checked')) {
            $("#DEPT_BLDG_LOC,#DEPT_BLDG_FLR,#DEPT_BLDG_OFFC_NUM").show(); // Dept Info

            $("#EMP_POSITION").show();
            $("#label_4b").show();
            $("#DEPT_BLDG_FLR,#DEPT_BLDG_LOC,#DEPT_BLDG_OFFC_NUM").show();
            $("#HWSW_HW_COST_CENTER,#HWSW_HW_GL_ACCT").show();
            $("#HWSW_HW_REQUEST_TYPE,#HWSW_HW_CURRENT_DEVICE_ID").show();
            $("#HWSW_HW_CHECKBOX_1,#HWSW_HW_CHECKBOX_2,#HWSW_HW_CHECKBOX_3").show();
            $("#HWSW_HW_CHECKBOX_4,#HWSW_HW_CHECKBOX_5").show();
        } else if ($(this).not(':checked')) {
            $("#DEPT_BLDG_LOC,#DEPT_BLDG_FLR,#DEPT_BLDG_OFFC_NUM").hide(); // Dept Info

            $("#EMP_POSITION").hide();
            $("#label_4b").hide();
            $("#DEPT_BLDG_FLR,#DEPT_BLDG_LOC,#DEPT_BLDG_OFFC_NUM").hide();
            $("#HWSW_HW_COST_CENTER,#HWSW_HW_GL_ACCT").hide();
            $("#HWSW_HW_REQUEST_TYPE,#HWSW_HW_CURRENT_DEVICE_ID").hide();
            $("#HWSW_HW_CHECKBOX_1,#HWSW_HW_CHECKBOX_2,#HWSW_HW_CHECKBOX_3").hide();
            $("#HWSW_HW_CHECKBOX_4,#HWSW_HW_CHECKBOX_5").hide();
        }
    });

    // Specialized Software
    $("#HWSW_REQUEST_SOFTWARE").click(function () {
        if ($(this).is(':checked')) {
            $("#label_4c").show();
            $("#HWSW_SW_COST_CENTER,#HWSW_SW_GL_ACCT").show();
            $("#HWSW_SW_NOT_STANDARD_BOOL,#HWSW_SW_NOT_STANDARD_BOOL_LABEL,#HWSW_SW_NOT_STANDARD_BOOL_LABEL2").show();
            $("#HWSW_SW_SELECT_1,#HWSW_SW_OTHER_DESC_1,#HWSW_SW_SELECT_QTY_ADOBEPRO").show();
            $("#HWSW_SW_SELECT_2,#HWSW_SW_OTHER_DESC_2,#HWSW_SW_SELECT_QTY_ADOBECREAT").show();
            $("#HWSW_SW_SELECT_3,#HWSW_SW_OTHER_DESC_3,#HWSW_SW_SELECT_QTY_MSVISIO").show();
            $("#HWSW_SW_SELECT_4,#HWSW_SW_OTHER_DESC_4,#HWSW_SW_SELECT_QTY_MSPROJECT").show();
            $("#HWSW_SW_SELECT_5,#HWSW_SW_OTHER_DESC_5,#HWSW_SW_SELECT_QTY_ADOBEPS").show();
            $("#HWSW_SW_SELECT_6,#HWSW_SW_OTHER_DESC_6,#HWSW_SW_SELECT_QTY_EXCHANGEMAIL").show();
            $("#HWSW_SW_SELECT_7,#HWSW_SW_OTHER_DESC_7,#HWSW_SW_SELECT_QTY_PREZIPRO").show();
            $("#HWSW_SW_SELECT_8,#HWSW_SW_OTHER_DESC_8,#HWSW_SW_SELECT_QTY_ADOBEACRO").show();
            $("#HWSW_SW_SELECT_9,#HWSW_SW_OTHER_DESC_9,#HWSW_SW_SELECT_QTY_ADOBEINDESIGN").show();
            $("#HWSW_SW_SELECT_10,#HWSW_SW_OTHER_DESC_10,#HWSW_SW_SELECT_QTY_VMWARE").show();
        } else if ($(this).not(':checked')) {
            $("#label_4c").hide();
            $("#HWSW_SW_COST_CENTER,#HWSW_SW_GL_ACCT").hide();
            $("#HWSW_SW_NOT_STANDARD_BOOL,#HWSW_SW_NOT_STANDARD_BOOL_LABEL,#HWSW_SW_NOT_STANDARD_BOOL_LABEL2").hide();
            $("#HWSW_SW_NOT_STANDARD_BOOL").prop("checked", false);
            $("#HWSW_SW_SELECT_1,#HWSW_SW_OTHER_DESC_1,#HWSW_SW_SELECT_QTY_ADOBEPRO").hide();
            $("#HWSW_SW_SELECT_2,#HWSW_SW_OTHER_DESC_2,#HWSW_SW_SELECT_QTY_ADOBECREAT").hide();
            $("#HWSW_SW_SELECT_3,#HWSW_SW_OTHER_DESC_3,#HWSW_SW_SELECT_QTY_MSVISIO").hide();
            $("#HWSW_SW_SELECT_4,#HWSW_SW_OTHER_DESC_4,#HWSW_SW_SELECT_QTY_MSPROJECT").hide();
            $("#HWSW_SW_SELECT_5,#HWSW_SW_OTHER_DESC_5,#HWSW_SW_SELECT_QTY_ADOBEPS").hide();
            $("#HWSW_SW_SELECT_6,#HWSW_SW_OTHER_DESC_6,#HWSW_SW_SELECT_QTY_EXCHANGEMAIL").hide();
            $("#HWSW_SW_SELECT_7,#HWSW_SW_OTHER_DESC_7,#HWSW_SW_SELECT_QTY_PREZIPRO").hide();
            $("#HWSW_SW_SELECT_8,#HWSW_SW_OTHER_DESC_8,#HWSW_SW_SELECT_QTY_ADOBEACRO").hide();
            $("#HWSW_SW_SELECT_9,#HWSW_SW_OTHER_DESC_9,#HWSW_SW_SELECT_QTY_ADOBEINDESIGN").hide();
            $("#HWSW_SW_SELECT_10,#HWSW_SW_OTHER_DESC_10,#HWSW_SW_SELECT_QTY_VMWARE").hide();
        }
    });

    // Something other than standard CoSA Software
    $("#HWSW_SW_NOT_STANDARD_BOOL").click(function () {
        if ($("#HWSW_SW_NOT_STANDARD_BOOL").is(':checked')) {
            $("#HWSW_SW_OTHER_IMAGE").show();
        } else if ($("#HWSW_SW_NOT_STANDARD_BOOL").not(':checked')) {
            $("#HWSW_SW_OTHER_IMAGE").hide();
        }
    });

    // Prov Request Type
    $("#PROV_REQUEST_TYPE").change(function () {
        console.log($("#PROV_REQUEST_TYPE").val());
        if ($("#PROV_REQUEST_TYPE").val() == 'PROVI_USER') {
            $("#label_5b,#label_5c,#label_5d,#label_5e").show(); // User Access, Access Requested, Additional Req, Mainframe
            $("#PROV_USER_TYPE,#PROV_HIRE_DATE_LABEL,#PROV_DEVICE_EXIST,#PROV_DEVICE_EXIST_BOOL_LABEL").show();
            $("#PROV_PHONE_EXIST_BOOL_LABEL,#PROV_PHONE_SET_ADMIN_BOOL_LABEL").show();

            $("#PROV_ACCESS_REQUEST_1,#PROV_ACCESS_REQUEST_2,#PROV_ACCESS_REQUEST_3").show(); // (H) (K) Email
            $("#PROV_ACCESS_REQUEST_HOME,#PROV_ACCESS_REQUEST_SHARE,#PROV_ACCESS_REQUEST_EMAIL").show(); // H K E Bools
            $("#PROV_ADDITIONAL_REQ,#PROV_MAINFRAME_ACCESS").show();
            // hide
            $("#PROV_DISABLE_DATE_LABEL").hide();
        } else if ($("#PROV_REQUEST_TYPE").val() == 'DISABLE') {
            // show
            $("#EMP_USERNAME").show();
            $("#PROV_DISABLE_DATE_LABEL").show();
            // hide
            $("#label_5b,#label_5c,#label_5d,#label_5e").hide(); // User Access, Access Requested, Additional Req, Mainframe
            $("#PROV_USER_TYPE,#PROV_HIRE_DATE_LABEL,#PROV_DEVICE_EXIST_BOOL_LABEL").hide();
            $("#PROV_DEVICE_ASSET_NUM,#PROV_DEVICE_EXIST").hide();
            $("#PROV_DEVICE_EXIST,#PROV_PHONE_EXIST,#PROV_PHONE_SET_ADMIN").prop("checked", false);
            $("#PROV_PHONE_EXIST_BOOL_LABEL,#PROV_PHONE_SET_ADMIN_BOOL_LABEL").hide();
            $("#PROV_PHONE_EXIST_FIELDS,#PROV_PHONE_SET_ADMIN_CONTACT").hide();

            $("#PROV_ACCESS_REQUEST_1,#PROV_ACCESS_REQUEST_2,#PROV_ACCESS_REQUEST_3").hide(); // (H) (K) Email
            $("#PROV_ACCESS_REQUEST_HOME,#PROV_ACCESS_REQUEST_SHARE,#PROV_ACCESS_REQUEST_EMAIL").hide();
            $("#PROV_ACCESS_REQUEST_HOME,#PROV_ACCESS_REQUEST_SHARE,#PROV_ACCESS_REQUEST_EMAIL").prop("checked", false);
            $("#PROV_PREV_NAME,#PROV_PREV_SAP").hide();

            $("#PROV_ADDITIONAL_REQ,#PROV_MAINFRAME_ACCESS").hide();
            $("#PROV_ADDITIONAL_REQ,#PROV_MAINFRAME_ACCESS").prop("checked", false);
            $("#PROV_ADDITIONAL_REQ2,#PROV_MAINFRAME_ACCESS2").hide();
            alert("This form is NOT used to Terminate User accounts. All User Account Terminations must be processed through their department's HR Representative(s).");
        }
    });


    // User Provisioning - Device Assignment
    $("#PROV_DEVICE_EXIST").click(function () {
        if ($(this).is(':checked')) {
            $("#PROV_DEVICE_ASSET_NUM").show();
            alert("Special Device Provisioning Notice: By selecting this option, you are planning to purchase " +
                "new equipment, and will need to select and fill-out the Hardware/Software selection on this form. ");
        } else if ($(this).not(':checked')) {
            $("#PROV_DEVICE_ASSET_NUM").hide();
        }
    });

    // User Provisioning - Phone Assignment
    $("#PROV_PHONE_EXIST").click(function () {
        if ($(this).is(':checked')) {
            $("#PROV_PHONE_EXIST_FIELDS").show();
        } else if ($(this).not(':checked')) {
            $("#PROV_PHONE_EXIST_FIELDS").hide();
        }
    });

    // User Provisioning - Phone Assignment
    $("#PROV_PHONE_SET_ADMIN").click(function () {
        if ($(this).is(':checked')) {
            $("#PROV_PHONE_SET_ADMIN_CONTACT").show();
        } else if ($(this).not(':checked')) {
            $("#PROV_PHONE_SET_ADMIN_CONTACT").hide();
        }
    });

    // User Access - Email; Previous name and SAP
    $("#PROV_ACCESS_REQUEST_EMAIL").click(function () {
        if ($(this).is(':checked')) {
            $("#PROV_PREV_NAME,#PROV_PREV_SAP").show();
        } else if ($(this).not(':checked')) {
            $("#PROV_PREV_NAME,#PROV_PREV_SAP").hide();
        }
    });

    // Additional Req
    $("#PROV_ADDITIONAL_REQ").click(function () {
        if ($(this).is(':checked')) {
            $("#PROV_ADDITIONAL_REQ2").show();
        } else if ($(this).not(':checked')) {
            $("#PROV_ADDITIONAL_REQ2").hide();
        }
    });

    // Mainframe Access
    $("#PROV_MAINFRAME_ACCESS").click(function () {
        if ($(this).is(':checked')) {
            $("#PROV_MAINFRAME_ACCESS2").show();
        } else if ($(this).not(':checked')) {
            $("#PROV_MAINFRAME_ACCESS2").hide();
        }
    });

    // MOUSE-OVER TEXT FOR SOFTWARE SELECTION
    $("#HWSW_SW_SELECT_1").hover(function () {
        $(this).attr('title', '    The complete PDF solution for working anywhere. Use \n' +
            'this desktop software to save PDFs as Microsoft Word, \n' +
            'Excel, or PowerPoint files. Edit PDF text and images.\n' +
            'Includes Export PDF, Adobe mobile app, PDF Pack, Adobe \n' +
            'Sign, and Acrobat Document Cloud.');
    });
    $("#HWSW_SW_SELECT_2").hover(function () {
        $(this).attr('title', '    The entire collection of 20+ creative desktop and mobile apps\n' +
            'including Photoshop CC, Illustrator CC, and Adobe XD CC');
    });
    $("#HWSW_SW_SELECT_3").hover(function () {
        $(this).attr('title', '    Easily create diagrams, organization charts, maps, workflows\n' +
            'and home or office plans using a rich set of shapes and \n' +
            'templates. A familiar Office experience allows you to perform \n' +
            'common flow charts with ease.');
    });
    $("#HWSW_SW_SELECT_4").hover(function () {
        $(this).attr('title', '    Project helps you stay organized with simple tools that make it\n' +
            'easy to get started, quickly plan, and keep your projects on track.');
    });
    $("#HWSW_SW_SELECT_5").hover(function () {
        $(this).attr('title', '    Capture attention with beautiful, high-impact visuals. Adjust \n' +
            'clarity, color and tone and create image manipulation effects or \n' +
            'perfect your graphic design work. Take your pictures to the next \n' +
            'level with the most powerful image editing software for web, \n' +
            'desktop, and mobile.');
    });
    $("#HWSW_SW_SELECT_6").hover(function () {
        $(this).attr('title', 'Create, configure, and use resource mailboxes');
    });
    $("#HWSW_SW_SELECT_7").hover(function () {
        $(this).attr('title', '    Create beautiful presentations on an online editor and control ' +
            'who sees them');
    });
    $("#HWSW_SW_SELECT_8").hover(function () {
        $(this).attr('title', 'Convert, sign, and send documents on any device.');
    });
    $("#HWSW_SW_SELECT_9").hover(function () {
        $(this).attr('title', '    Create, preflight, and publish beautiful documents for print and \n' +
            'digital media. Make posters, books, digital magazines, eBooks, \n' +
            'interactive PDFs, and more');
    });
    $("#HWSW_SW_SELECT_10").hover(function () {
        $(this).attr('title', 'Desktop virtualization features and cloud capabilities');
    });

});