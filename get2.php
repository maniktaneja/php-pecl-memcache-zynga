<?php
$memcache = new memcache;
$memcache->addServer("localhost", 11211);

if ($memcache) {
	$val = null;

	$key = "foo"; // non-existant key
    $flag = 0;
    $cas = 0;
	$test = $memcache->get2($key, $val, $flag, $cas);
    var_dump($test); // true if call succeeded. false if any error happened during the call, like network error etc. Check the remaining values below only if this value is true.
    var_dump($key);  
    var_dump($val);  // value if it exists, NULL otherwise.
    var_dump($flag); // flag if value exists, uninitialized junk otherwise
    var_dump($cas);  // cas if value exists,  uninitialized junk otherwise
} else {
	echo "failed to create memcache client\n";
}
