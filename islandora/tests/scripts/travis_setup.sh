#!/bin/bash

mysql -u root -e 'create database drupal;'
mysql -u root -e "create database fedora;"
mysql -u root -e "GRANT ALL PRIVILEGES ON fedora.* To 'fedora'@'localhost' IDENTIFIED BY 'fedora';"
mysql -u root -e "GRANT ALL PRIVILEGES ON drupal.* To 'drupal'@'localhost' IDENTIFIED BY 'drupal';"
cd $HOME
git clone git://github.com/Islandora/tuque.git
git clone -b $FEDORA_VERSION git://github.com/Islandora/islandora_tomcat.git
cd islandora_tomcat
export CATALINA_HOME='.'
export JAVA_OPTS="-Xms1024m -Xmx1024m -XX:MaxPermSize=512m -XX:+CMSClassUnloadingEnabled -Djavax.net.ssl.trustStore=$CATALINA_HOME/fedora/server/truststore -Djavax.net.ssl.trustStorePassword=tomcat"
./bin/startup.sh
cd $HOME
pear upgrade --force Console_Getopt
pear upgrade --force pear
pear channel-discover pear.drush.org
pear channel-discover pear.drush.org
pear channel-discover pear.phpqatools.org
pear channel-discover pear.netpirates.net
pear install pear/PHP_CodeSniffer
pear install pear.phpunit.de/phpcpd

# Install Drush
git clone https://github.com/drush-ops/drush.git
pushd drush
git checkout 5.9.0
chmod +x drush
popd
sudo ln -s $HOME/drush/drush /usr/local/sbin

phpenv rehash
drush dl --yes drupal
cd drupal-*
drush si minimal --db-url=mysql://drupal:drupal@localhost/drupal --yes
drush runserver --php-cgi=$HOME/.phpenv/shims/php-cgi localhost:8081 &>/dev/null &
ln -s $ISLANDORA_DIR sites/all/modules/islandora
mv sites/all/modules/islandora/tests/travis.test_config.ini sites/all/modules/islandora/tests/test_config.ini
mkdir sites/all/libraries
ln -s $HOME/tuque sites/all/libraries/tuque
drush dl --yes coder
drush dl --yes potx
drush en --yes coder_review
drush en --yes simpletest
drush en --yes potx
drush en --user=1 --yes islandora
drush cc all
sleep 20
