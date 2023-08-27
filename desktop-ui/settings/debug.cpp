auto DebugSettings::construct() -> void {
  setCollapsible();
  setVisible(false);

  debugLabel.setText("GDB-Server").setFont(Font().setBold());
  portLayout.setAlignment(1);
    portLabel.setText("Port");

    port.setText(integer(settings.debugServer.port));
    port.setEditable(true);
    port.onChange([&](){
      settings.debugServer.port = port.text().integer();
      string portStr = integer(settings.debugServer.port);

      if(portStr != port.text()) {
        port.setText(settings.debugServer.port == 0 ? string{""} : portStr);
      }

      infoRefresh();
    });

    portHint.setText("Safe range: 1024 - 32767").setFont(Font().setSize(7.0)).setForegroundColor(SystemColor::Sublabel);

  tokenLayout.setAlignment(1);
    tokenLabel.setText("Token");

    token.setText(settings.debugServer.token);
    token.setEditable(true);
    token.onChange([&](){
      settings.debugServer.token = token.text();
    });
    tokenHint.setText("Token for HTTP connections").setFont(Font().setSize(7.0)).setForegroundColor(SystemColor::Sublabel);

  ipv4Layout.setAlignment(1);
    ipv4Label.setText("Use IPv4");

    ipv4.setEnabled(true);
    ipv4.setChecked(settings.debugServer.useIPv4);
    ipv4.onToggle([&](){
      settings.debugServer.useIPv4 = ipv4.checked();
      serverRefresh();
      infoRefresh();
    });

  enabledLayout.setAlignment(1);
    enabledLabel.setText("Enabled");

    enabled.setEnabled(true);
    enabled.setChecked(settings.debugServer.enabled);
    enabled.onToggle([&](){
      settings.debugServer.enabled = enabled.checked();
      serverRefresh();
      infoRefresh();
    });

  infoRefresh();
}

auto DebugSettings::infoRefresh() -> void {
  if(settings.debugServer.enabled) {
    string url{
      settings.debugServer.useIPv4 ? "127.0.0.1:" : "[::1]:", 
      integer(settings.debugServer.port)
    };
    connectInfo.setText({"To connect, start gdb-multiarch and run: \"target remote ", url, "\""});
    presentation.statusDebug.setText({"GDB active at ", url});
  } else {
    connectInfo.setText("");
    presentation.statusDebug.setText("");
  }
}

auto DebugSettings::serverRefresh() -> void {
  ares::GDB::server.close();

  if(settings.debugServer.enabled) {
    ares::GDB::server.open(settings.debugServer.port, settings.debugServer.useIPv4);
  }
}
