--TEST--
check setMultiByKey method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache();
$memcache->addServer("192.168.1.4",11211);

$val = null;

$key = "foo"; // non-existant key
$flag = 0;
$cas = 0;
$test = $memcache->get2($key, $val, $flag, $cas);
var_dump($test); // true if call succeeded. false if any error happened during the call, like network error etc. Check the remaining values below only if this value is true.
var_dump($key);  
var_dump($val);  // value if it exists, NULL otherwise.
var_dump($flag); // flag if value exists, uninitialized junk otherwise
var_dump($cas != 0);  // cas if value exists,  uninitialized junk otherwise

--EXPECT--
bool(true)
string(3) "foo"
string(5) "value"
int(0)
bool(true)
