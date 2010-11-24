<?php

$memcache = new memcache;

$memcache->addServer("localhost", 11211);


if ($memcache) {

    $get_array = array("k1","k2","k3","k4","k5");
    $memcache->delete("k1");
    $memcache->getl("k1", 20);

    $memcache->getl("x1");

    $memcache->set("k1", "whats her name");

    $memcache->getl("k1");
    $memcache->set("k1", "dont be late, alright");

    $memcache->getl("k1"); /* should fail */

    echo "haha";
    var_dump($memcache->getl("k1"));
    $memcache->set("k1", "lizzi the wizzi");

    var_dump($memcache->get('k1'));

    $memcache->set("k1", 25);
    $memcache->getl("k1");
    $memcache->increment("k1", 5);
    $memcache->add("k1", 50, 0);
    $memcache->delete("k1");

    $return = $memcache->getl("z1");
    var_dump($return);


}
else {
	echo "Connection to memcached failed";
}
?>

