# IRCSTATUS

The _ircstatus_ is a [pidgin](https://pidgin.im) plugin to change the IRC nick of one of your accounts when changing your general status in your application. The new nick is obtained by the concatenation of <username> + <separator> + <status>:

* username: It is the username configured in the IRC account selected.
* separator: It is the string configured in the preferences (by default "|").
* status: It is the status title but sanitized (all non-alphanumeric characters are replaced by an underscore "_").

The plugin just works for one of your IRC accounts (you can configure which one in the preferences).

In order to configure the plugin you need the autotools packages:

1. Clone the repository:

```bash
git clone https://github.com/rickyepoderu/ircstatus
```

2. Execute the bootstrap command in the plugin root directory:

```bash
cd ircstatus
./bootstrap
```

3. Configure, make and make install (bu default the resulting library is installed in the user plugin directory for pidgin _~/.purple/plugins/_):

```bash
./configure
make
make install
```

4. Restart pidgin and configure your IRC account in the Tools -> Plugins -> IRC Status preferences page.

