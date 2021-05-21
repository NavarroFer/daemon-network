const dbus = require("dbus-next");
const fs = require('fs');
const bus = dbus.systemBus();

var daemon = require('./daemon')

var dirLog = '/var/log/network-daemon/'

const estados = {
    OFFLINE: 0,
    ONLINE: 1
}

const NMState = {
    NM_STATE_UNKNOWN: 0,
    NM_STATE_ASLEEP: 10,
    NM_STATE_DISCONNECTED: 20,
    NM_STATE_DISCONNECTING: 30,
    NM_STATE_CONNECTING: 40,
    NM_STATE_CONNECTEEEED_LOCAL: 50,
    NM_STATE_CONNECTED_SITE: 60,
    NM_STATE_CONNECTED_GLOBAL: 70
}

var estadoActual = estados.OFFLINE;

async function main() {
    let obj = await bus.getProxyObject('org.freedesktop.NetworkManager', '/org/freedesktop/NetworkManager');
    let properties = obj.getInterface('org.freedesktop.DBus.Properties');

    daemon();

    properties.on('PropertiesChanged', (iface,changed,invalidated) => {
        let state = changed.State;
        if(typeof state !== 'undefined') {
            // Significa que hay conexión global
            if(state.value == NMState.NM_STATE_CONNECTED_GLOBAL) {
                if( estadoActual == estados.OFFLINE) {
                    estadoActual = estados.ONLINE;
                    registrarMensaje('Hay conexión a internet.');
                }
            }
            else { // significa que no hay conexión global
                if(estadoActual == estados.ONLINE) {
                    estadoActual = estados.OFFLINE;
                    registrarMensaje('Se fue la conexión a internet.');
                }
            }
        }
    });
}

main();

function registrarMensaje(m) {
    console.log(m);
    var log_file = fs.createWriteStream(dirLog+'logConexion3.txt', {flags: 'a'});
    log_file.write("[" + new Date() + "]: " + m + "\n");
}
