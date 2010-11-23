rm -rf *.rpm;
./build-rpm.sh;
sudo rpm -e php-pecl-memcache-zynga.x86_64;
sudo rpm -i php-pecl-memcache-zynga-2.4.0.0-5.2.10.x86_64.rpm;
sudo rpm -q php-pecl-memcache-zynga.x86_64;
sudo /sbin/service httpd restart
echo "Done";
