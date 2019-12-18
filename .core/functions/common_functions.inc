<?php

function isVersionLive()
{
    $live_version = false;
    $yes_variants = array(
        'on',
        'yes',
        '1',
    );

    if(in_array(LIVE_VERSION, $yes_variants)){
        $live_version = true;
    }
    return $live_version;
}

function fCheckEmail($email)
{
     if( preg_match('/^(?:[\w\!\#\$\%\&\'\*\+\-\/\=\?\^\`\{\|\}\~]+\.)*[\w\!\#\$\%\&\'\*\+\-\/\=\?\^\`\{\|\}\~]+@(?:(?:(?:[a-zA-Z0-9_](?:[a-zA-Z0-9_\-](?!\.)){0,61}[a-zA-Z0-9_-]?\.)+[a-zA-Z0-9_](?:[a-zA-Z0-9_\-](?!$)){0,61}[a-zA-Z0-9_]?)|(?:\[(?:(?:[01]?\d{1,2}|2[0-4]\d|25[0-5])\.){3}(?:[01]?\d{1,2}|2[0-4]\d|25[0-5])\]))$/', $email) === 0){
        return false;
    }else{
        return true;
    }
}

/*Convert php timestamp to Mysql query format string. Will work with time also*/
function phpdate2mysqldate($phpdate){
    return date( 'Y-m-d H:i:s', $phpdate );
}

//Convert 2009-06-15 13:16:4 to timestamp
function mysqldate2phpdate($mysqldate){
    $phpdate = strtotime( $mysqldate );
    return $phpdate;
}
