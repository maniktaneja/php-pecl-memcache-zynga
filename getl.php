<?php

$memcache = new memcache;

$memcache->addServer("localhost", 11411);


if ($memcache) {

    $get_array = array("k1","k2","k3","k4","k5");
    $memcache->delete("k1");
    $memcache->getl("k1");

    $memcache->set("k1", "whats her name");

    $memcache->getl("k1");
    $memcache->set("k1", "dont be late, alright");

    $memcache->getl("k1"); /* should fail */

    $memcache->getl("k1"); /* should succeed */
    $memcache->set("k1", "lizzi the wizzi");

    var_dump($memcache->get('k1'));

}
else {
	echo "Connection to memcached failed";
}
?>

