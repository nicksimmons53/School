var api_key = '9879d49682d1f60c68ab1ba1f376b5c3-8b7bf2f1-5c36afbe';
var domain = 'sandboxb9e280a1cac14b909500c445f1224f24.mailgun.org';
var mailgun = require('mailgun-js')({ apiKey: api_key, domain: domain });

var validator = require('./validator');
var errorMessages = [ ];

module.exports = {
    validate: function(req) {
        req.check('EMP_FIRST', 'EMP_FIRST').isLength({min : 0}).isAlpha( );
        req.check('EMP_MI', 'EMP_MI').optional({ checkFalsy: true }).isAlpha( );
        req.check('EMP_LAST', 'EMP_LAST').isLength({min : 0}).isAlpha( );
        req.check('EMP_PHONE', 'EMP_PHONE').isInt( );
        req.check('EMP_DEPT', 'EMP_DEPT').notEmpty( );
        req.check('DEPT_PHONE_EXT', 'DEPT_PHONE_EXT').isInt( );
        req.check('EMP_DIVISION', 'EMP_DIVISION').notEmpty( );
        req.check('DEPT_COST_CENTER', 'DEPT_COST_CENTER').notEmpty( );
        req.check('EMP_SAP', 'EMP_SAP').isLength(6).isInt( );

        var errors = req.validationErrors( );

        for (i = 0; i < errors.length; i++) {
            var message = errors[i].msg;
            errorMessages.push(message);
        }

        if (errors) {
            // Declare and initialize required email variables
            var data = {
                from: 'Do Not Reply <autobeanteam@gmail.com>',
                to: 'autobeanteam@gmail.com',
                subject: 'Provision Form',
                text: 'To whomever is managing errors, we may have detected some errors:\n' + errorMessages + '\n\nFollow this link to view the form:    http://localhost:3306/'
            };

            // Use mailgun to send emails
            mailgun.messages().send(data, function (error, body) {
                if (error) {
                    console.log(error);
                }
                console.log(body);
            });
        }
        if (true) {
            // Declare and initialize required email variables
            var data = {
                from: 'Do Not Reply <autobeanteam@gmail.com>',
                to: 'autobeanteam@gmail.com',
                subject: 'Provision Form',
                text: 'Fiscal Administrator, you have a new provisioning form for approval.\n\nFollow this link to view the form:    http://localhost:3306/'
            };

            // Use mailgun to send emails
            mailgun.messages().send(data, function (error, body) {
                if (error) {
                    console.log(error);
                }
                console.log(body);
            });
        }
    }
}
