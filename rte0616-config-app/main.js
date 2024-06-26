const { app, BrowserWindow } = require("electron");
const path = require("path");
const url = require("url");

let win;
function createWindow() {
  win = new BrowserWindow({ width: 1050, height: 850 , autoHideMenuBar: true});
  // load the dist folder from Angular
  const appUrl = url.format({
    pathname: path.join(__dirname, "/dist/rte0616-config-app/browser/index.html"),
    protocol: "file:",
    slashes: true
  });

  win.loadURL(appUrl);

  // Workaround for reload menu.
  win.webContents.on('did-fail-load', () => win.loadURL(appUrl));

  // The following is optional and will open the DevTools:
  // win.webContents.openDevTools()
  win.on("closed", () => {
    win = null;
  });
}
app.on("ready", createWindow);
// on macOS, closing the window doesn't quit the app
app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

