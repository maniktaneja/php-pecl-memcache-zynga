<?php

$memcache = new memcache;

$memcache->addServer("localhost", 11211);


if ($memcache) {

    $memcache->set("k1", "whats her name");
    $memcache->append("k1", "? pappu yadav ");
    var_dump($memcache->get("k1"));

    $result = $memcache->append("k9", "rather stupid idea"); /* should fail */
    if ($result == false) {
        echo "pass ! \n";
    }

    $result = $memcache->prepend("k9", "rather silly idea"); /* should fail */
    echo $result;

    if ($result == false) {
        echo "pass ! \n";
    }

    $memcache->prepend("k1", "who wrote toy soldiers ? ");
    var_dump($memcache->get("k1"));


/*
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
*/

}
else {
	echo "Connection to memcached failed";
}
?>

