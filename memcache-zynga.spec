%{!?__pecl:     %{expand: %%global __pecl     %{_bindir}/pecl}}
%{!?php_extdir: %{expand: %%global php_extdir %(php-config --extension-dir)}}
%global php_apiver  %((echo 0; php -i 2>/dev/null | sed -n 's/^PHP API => //p') | tail -1)
%global php_version  %((echo 0; php -i 2>/dev/null | sed -n 's/^PHP Version => //p') | tail -1)

%define pecl_name memcache-zynga
%define module_name memcache
%define log_dir /etc/pecl-log 

Summary:      Memcached extension with custom changes for zynga
Name:         php-pecl-memcache-zynga
Version:      2.5.0.3
Release:      %{?php_version}
License:      PHP
Group:        Development/Languages
URL:          http://pecl.php.net/package/%{pecl_name}

Source:       http://pecl.php.net/get/%{pecl_name}-%{version}.tgz

Source2:      xml2changelog

BuildRoot:    %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildRequires: php-devel >= 4.3.11, php-pear, zlib-devel
Requires(post): %{__pecl}
Requires(postun): %{__pecl}
Provides:     php-pecl(%{pecl_name}) = %{version}-%{release}
%if %{?php_zend_api}0
Requires:     php(zend-abi) = %{php_zend_api}
#Requires:     php(api) = %{php_core_api}
%else
#Requires:     php-api = %{php_apiver}
%endif
Requires:     php >= %{php_version}, php-igbinary
Conflicts:    php-pecl(memcache)

%description
Memcached is a caching daemon designed especially for
dynamic web applications to decrease database load by
storing objects in memory.

This extension allows you to work with memcached through
handy OO and procedural interfaces.

Zynga customization - proxy support added

Memcache can be used as a PHP session handler.

%prep
%setup -c -q
%{_bindir}/php -n %{SOURCE2} package.xml >CHANGELOG

# avoid spurious-executable-perm
find . -type f -exec chmod -x {} \;


%build
cd %{pecl_name}-%{version}
phpize
chmod +x ./configure
%configure
%{__make} %{?_smp_mflags}


%install
cd %{pecl_name}-%{version}
%{__rm} -rf %{buildroot}
%{__make} install INSTALL_ROOT=%{buildroot}

# Drop in the bit of configuration
%{__mkdir_p} %{buildroot}%{_sysconfdir}/php.d
%{__cat} > %{buildroot}%{_sysconfdir}/php.d/%{module_name}.ini << 'EOF'
; Enable %{pecl_name} extension module
extension=%{module_name}.so

; Options for the %{module_name} module

; Whether to transparently failover to other servers on errors
;memcache.allow_failover=1
; Defines how many servers to try when setting and getting data.
;memcache.max_failover_attempts=20
; Data will be transferred in chunks of this size
;memcache.chunk_size=8192
; The default TCP port number to use when connecting to the memcached server
;memcache.default_port=11211
; Hash function {crc32, fnv}
;memcache.hash_function=crc32
; Hash strategy {standard, consistent}
;memcache.hash_strategy=standard

; Options for enabling proxy

; Enables/disabled proxy support
;memcache.proxy_enabled=false
; Proxy host/ip. For unix domain sockets, use unix://<abs path to socket>
;memcache.proxy_host=localhost
; Proxy port. For unix domain sockets, give 0
;memcache.proxy_port=0

; Compression level - ignored unless compression threshold is set on the memcache pool
; 0: use LZO compression
; 1 - 9: use Zlib compression
;memcache.compression_level=0

; Return value of get() on error
; By default, get() returns FALSE for both error and key miss
; with null_on_keymiss=1, get returns null for key miss and FALSE for error
;memcache.null_on_keymiss=0

; Options to use the memcache session handler

; Use memcache as a session handler
;session.save_handler=memcache
; Defines a comma separated of server urls to use for session storage
;session.save_path="tcp://localhost:11211?persistent=1&weight=1&timeout=1&retry_interval=15"
; Option to enable the number of retries on a persistent connection
;memcache.connection_retry_count=0
memcache.log_conf = %{log_dir}/logconf
; Option to enable end to end Data Integrity
; memcache.data_integrity_enabled=true
; Option to specify the algorithm used for data integrity, allowed values: "crc32", "none"
; memcache.integrity_algo="crc32"
EOF

# Install XML package description
# use 'name' rather than 'pecl_name' to avoid conflict with pear extensions
%{__mkdir_p} %{buildroot}%{pecl_xmldir}
#%{__install} -m 644 ../package.xml %{buildroot}%{pecl_xmldir}/%{name}.xml
%{__install} -m 644 package.xml %{buildroot}%{pecl_xmldir}/%{name}.xml

%{__mkdir_p} %{buildroot}%{log_dir}
%{__cat} > %{buildroot}%{log_dir}/logconf << 'EOF'
default
EOF
%{__mkdir_p} %{buildroot}/etc/logrotate.d
%{__cat} > %{buildroot}/etc/logrotate.d/pecl << 'EOF'
/var/log/pecl-memcache.log /var/log/pecl-apache.log {
  sharedscripts
    notifempty
    missingok
    size 100M
    compress 
    postrotate
        /bin/kill -HUP `cat /var/run/syslogd.pid 2> /dev/null` 2> /dev/null || true
    endscript
}
EOF

%clean
%{__rm} -rf %{buildroot}


%if 0%{?pecl_install:1}
%post
%{pecl_install} %{pecl_xmldir}/%{name}.xml >/dev/null || :
if [ -f /etc/syslog-ng/syslog-ng.conf ];then
sed /pecl_.*log/d /etc/syslog-ng/syslog-ng.conf > /tmp/back
mv /tmp/back /etc/syslog-ng/syslog-ng.conf
echo 'destination d_pecl_log { file("/var/log/pecl-memcache.log" owner("root") group ("root") perm(0644) ); }; 
filter f_pecl_log { facility(local4) and level(debug) and match('pecl-memcache'); };                             
log { source(s_sys); filter(f_pecl_log); destination(d_pecl_log); };

destination d_pecl_apache_log { file("/var/log/pecl-apache.log" owner("root") group ("root") perm(0644) ); }; 
filter f_pecl_apache_log { facility(local5) and level(debug) and match('pecl-memcache'); };         
log { source(s_sys); filter(f_pecl_apache_log); destination(d_pecl_apache_log); };' >> /etc/syslog-ng/syslog-ng.conf
fi
/sbin/service syslog-ng restart      
%endif


%if 0%{?pecl_uninstall:1}
%postun
if [ $1 -eq 0 -a -x %{__pecl} ] ; then
    %{pecl_uninstall} %{pecl_name} >/dev/null || :
fi
%endif


%files
%defattr(-, root, root, -)
%doc CHANGELOG %{pecl_name}-%{version}/CREDITS %{pecl_name}-%{version}/README
%doc %{pecl_name}-%{version}/example.php %{pecl_name}-%{version}/memcache.php
%config(noreplace) %{_sysconfdir}/php.d/%{module_name}.ini
%{php_extdir}/%{module_name}.so
%{pecl_xmldir}/%{name}.xml
%{log_dir} 
/etc/logrotate.d/pecl


%changelog

* Tue Jul 03 2012 <vnatarajan@zynga.com> 2.5.0.0
- Data integrity support

* Fri Jun 01 2012 <nigupta@zynga.com> 2.4.1.17
- Adding lock metadata changes

* Fri Jun 01 2012 <nigupta@zynga.com> 2.4.1.16
- Fixing bugs in pecl-memcache

* Wed Apr 04 2012 <vnatarajan@zynga.com> 2.4.1.15
- Fixing multiget and multigetByKey for slingo crash

* Wed Apr 04 2012 <nigupta@zynga.com> 2.4.1.14
- Request logging changes for pecl-memcache

* Tue Mar 27 2012 <nigupta@zynga.com> 2.4.1.13
- cas2 changes for pecl-memcache with igbinary

* Wed Mar 21 2012 <nigupta@zynga.com> 2.4.1.12
igbinary and bzip2 changes for pecl-memcache

* Fri Jan 20 2012 <nigupta@zynga.com> 2.4.1.11
- Refactoring logging changes and few minor bug fixes.Removed the dependency on libzparse.

* Fri Dec 20 2011 <nigupta@zynga.com> 2.4.1.10
- Logging changes for pecl-memcache. Now pecl-memcache has dependency on libzparse.

* Fri Dec 03 2011 <mtaneja@zynga.com> 2.4.1.9
 - Fix read access violation due to incorrect calculation of value_len in
   mmc_pool_store

* Wed Nov 16 2011 <mtaneja@zynga.com> 2.4.1.8
 - Reverse all TSRM related changes

* Wed Nov 02 2011 <mtaneja@zynga.com> 2.4.1.7
- Add a setproperty option 'EnableChecksum'. When this property is set,
  crc checksums of both the compressed and uncompressed data will be
  prepended to the outgoing payload. These checksums if present are verified
  on the corresponding fetch operation
- Misc Fixes
  + Move test files into test directory
  + Fix crash in php_shutdown
  + Correct use thread safe TSRM constructs
  + Fix memory leak when multiGet fails

* Mon Oct 11 2011 <nigupta@zynga.com> 2.4.1.6
- Return the compressed value len by reference for add/set/replace/cas operations.

* Mon Aug 29 2011 <mtaneja@zynga.com> 2.4.1.5
- Reinitialize mmc->proxy pointers in get_stats and get_version since those pointers
  may be referencing freed objects. A multiget call creates a bunch of tmp proxy
  structures that are freed at the end of the multiget. However the mmc->proxy pointers
  that are pointing to those objects are not initialized to null

* Fri Jul 22 2011 <vsatyanarayana@zynga.com> 2.4.1.4
- Added a key and server info the 'unable to unserialize' log message.
- Added a key and server info the 'unable to uncompress' log message.

* Tue Apr 19 2011 <vsatyanarayana@zynga.com> 2.4.1.3
- Fix for a infinite loop in get_by_key api.
- Return false if uncompress failed in get_by_key.
- status_array was not NULL checked in a couple of places.

* Tue Mar 01 2011 <mtaneja@zynga.com> 2.4.1.2-1
- Weight paramter in addServer cannot be 0
- retry interval is a configurable paramter

* Mon Jan 24 2011 <mtaneja@zynga.com> 2.4.1.1-1
- PrependByKey was not using the shard key
- Shard Keys for xxxMultiByKey can be numeric

* Fri Jan 21 2011 <mtaneja@zynga.com> 2.4.1.0-1
- Append/Prepend operation will not compress data even if the compression
  flags are set.
- Fixed a crash in get_by_multi_key, which was happening due to a use of
  an uninitialized array variable
- Fixed a memory leak in get_by_multi_key

* Tue Jan 18 2011 <mtaneja@zynga.com> 2.4.0.3-1
- Added support for append(), prepend() and corresponding ByKey functions

* Mon Nov 29 2010 <mtaneja@zynga.com> 2.4.0.2-1
- Fixed another memory leak in multi-get introduced in 2.3.1.2

* Wed Nov 25 2010 <mtaneja@zynga.com> 2.4.0.1-1
- Fixed memory leak due to array_init in mmc_pool_new

* Wed Nov 18 2010 <bryan@zynga.com> 2.4.0.0-1
- Added all of the ByKey operations allowing you to pass in a separate shard
  key for identifying the memcache node where the operation will be performed

* Tue Nov 16 2010 <vsatyanarayana@zynga.com> 2.3.1.3
- A new function unlock() is now supported to unlock a getl'ed key
- 'add' will not be converted to cas
- Handling of "temporary failure" for incr/decr & delete
- getl on non existent key returns NULL
- getl can accept an optional timeout parameter

* Wed Nov 10 2010 <mtaneja@zynga.com> 2.3.1.2-1
- If uncompression or unserialization fails do not deactivate the server.
  Return and log error only.
- In case of multiget indicate failure of the keys that failed to uncompress/
  unserialize in the returned status array.
* Thu Sep 26 2010 (autumn equinox) <mtaneja@zynga.com> 2.3.1.1-1
- Added setProperty paramter ProtocolBinary (bool) to enable binary protocol
- Optional paramter to addServer and connect, use_binary to enable binary
  protocol for a server

* Fri Sep 24 2010 Mark Jaffe <mjaffe@zynga.com> 2.3.1.0-2
- Match the version string
* Wed Aug 25 2010 Manik Taneja <mtaneja@zynga.com> 2.3.1.0-1
- Implementation of getl(). This function takes the same number
  of parameters as get(). On success the object is locked for a
  default period of 15 seconds.
* Wed Aug 11 2010 Jayesh Jose <jjose@zynga.com> 2.3.0.10-1
- Fixed a crash that happens during module_shutdown()

* Mon Aug 09 2010 Manik Taneja <mtaneja@zynga.com> 2.3.0.9-1
- During a multi-get if a connection to the proxy fails, then
  do not try and open any more connections for that page

* Mon Jun 28 2010 Jayesh Jose <jjose@zynga.com> 2.3.0.8-1
- Fixed a crash in get2 when php serialized objects are retrieved

* Fri May 15 2010 Jayesh Jose <jjose@zynga.com> 2.3.0.4-1
- Fixed an issue with multi-get2 - with mcmux, it used to
- return failure even if just one of the keys failed

* Sat May 01 2010 Manik Taneja <mtaneja@zynga.com> 2.3.0.2-1
- multi-get2 returns with an array of key-status pairs
- added "non-numeric" return check for incr/decr

* Tue Apr 27 2010 Jayesh Jose <jjose@zynga.com> 2.3.0.1-1
- CAS support added
- New function, get2 that takes value as a ref param

* Wed Apr 21 2010 Jayesh Jose <jjose@zynga.com> 2.2.7.0-1
- Support for turning on/off proxy at a Memcache object level

* Wed Mar 10 2010 Jayesh Jose <jjose@zynga.com> 2.2.6.0-1
- Adding LZO compression support

* Thu Feb 25 2010 Prashun Purkayastha <ppurkayastha@zynga.com> 2.2.5.5-1
- Added a display message for the NOT_FOUND error

* Tue Feb 16 2010 Prashun Purkayastha <ppurkayastha@zynga.com> 2.2.5.4-1
- Added INI_SET option to retry with "memcache.connection_retry_count" on a
- failed set or get operation on a persistent connection

* Wed Nov 18 2009 Jayesh Jose <jjose@zynga.com> 2.2.5.3-1
- Disabled fall back to direct mc connection

* Thu Oct 29 2009 Jayesh Jose <jjose@zynga.com> 2.2.5-1
- Zynga version with proxy support

* Sat Feb 28 2009 Remi Collet <Fedora@FamilleCollet.com> 2.2.5-1
- new version 2.2.5 (bug fixes)

* Fri Sep 11 2008 Remi Collet <Fedora@FamilleCollet.com> 2.2.4-1
- new version 2.2.4 (bug fixes)

* Sat Feb  9 2008 Remi Collet <Fedora@FamilleCollet.com> 2.2.3-1
- new version

* Thu Jan 10 2008 Remi Collet <Fedora@FamilleCollet.com> 2.2.2-1
- new version

* Thu Nov 01 2007 Remi Collet <Fedora@FamilleCollet.com> 2.2.1-1
- new version

* Sat Sep 22 2007 Remi Collet <Fedora@FamilleCollet.com> 2.2.0-1
- new version
- add new INI directives (hash_strategy + hash_function) to config
- add BR on php-devel >= 4.3.11

* Mon Aug 20 2007 Remi Collet <Fedora@FamilleCollet.com> 2.1.2-1
- initial RPM

