--TEST--
check append prepend method
--SKIPIF--
<?php if (!extension_loaded("memcache")) print "skip"; ?>
--FILE--
<?php
$memcache = new Memcache;
$memcache->addServer("192.168.1.4", 11211);


$memcache->set("ak1", "whats her name");
$memcache->append("ak1", "? pappu yadav ");
var_dump($memcache->get("ak1"));

$result = $memcache->append("ak9", "rather stupid idea"); /* should fail */
if ($result == false) {
    echo "pass ! \n";
}

$result = $memcache->prepend("ak9", "rather silly idea"); /* should fail */
echo $result;

if ($result == false) {
    echo "pass ! \n";
}

$memcache->prepend("ak1", "who wrote toy soldiers ? ");
var_dump($memcache->get("ak1"));


/*
echo "haha";
var_dump($memcache->getl("ak1"));
$memcache->set("ak1", "lizzi the wizzi");

var_dump($memcache->get('ak1'));

$memcache->set("ak1", 25);
$memcache->getl("ak1");
$memcache->increment("ak1", 5);
$memcache->add("ak1", 50, 0);
$memcache->delete("ak1");

$return = $memcache->getl("az1");
var_dump($return);
*/
--EXPECT--
string(28) "whats her name? pappu yadav "
pass ! 
pass ! 
string(53) "who wrote toy soldiers ? whats her name? pappu yadav "

