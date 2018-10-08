/^[a-z]{3}[0-9]{3}/ {
    if ($1 in usernames) {
        usernames[$1] += 1;
        loginTime = substr($NF, 2, 5);
        split(timeSpent[$1], arr1, ":");
        split(loginTime, arr, ":");
        loginHour = arr[1] + arr1[1];
        loginMin = arr[2] + arr1[2];
        if (loginMin >= 60) {
            loginHour += 1;
            loginMin -= 60;
        }
        timeSpent[$1] = loginHour ":" loginMin;
    }
    else {
        usernames[$1] = 1;
        hostnames[$1] = $3;
        if ($NF != "in") {
            time = substr($NF, 2, 5);
            timeSpent[$1] = time;
        } else {
            loginTime = substr($7, 1, 5);
            split(loginTime, arr, ":");
            loginHour = currHour - arr[1];
            loginMin = currMin - arr[2];
            if (loginMin < 0) {
                loginHour -= 1;
                loginMin += 60;
            }
            timeSpent[$1] = loginHour ":" loginMin;
        }
    }
}
END {
    for (user in usernames) {
        printf("User: %s\n", user);
        printf("\tLast host: %s\n", hostnames[user]);
        printf("\tTotal Time: %s\n", timeSpent[user]);
        printf("\tTotal Sessions: %s\n", usernames[user]);
    }
}
