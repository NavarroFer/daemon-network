# daemon-network
El siguiente daemon, tiene la funcionalidad de escribir en /var/log/ARCHIVO_LOG la actividad de desconexion/conexion de internet.

Para el mismo, realizamos dos implementaciones:
   * C
   * NodeJs

## Uso
Para usar la implementacion en C:

1. Clonar el repositorio
   ```sh
   git clone https://github.com/NavarroFer/daemon-network
   ```
2. Compilar el codigo fuente
   ```sh
   gcc -o ChequeoRed ChequeoRed.c
   ```
3. Ejecutar ChequeoRed

Para usar la implementacion en NodeJs:

1. Clonar el repositorio
   ```sh
   git clone https://github.com/NavarroFer/daemon-network
   ```
2. Instalar NPM packages
   ```sh
   npm install dbus-next
   ```

3. Crear directorio para logs dentrto de /var/log/:
   ```sh
   sudo mkdir /var/log/network-daemon
   ```

4. Transferir propiedad al `usuario` que ejecutará el daemon:

   ```sh
   sudo chown usuario /var/log/network-daemon
   ```

5. Ejecutar ChequeoRed.js
   ```sh
   node ChequeoRed.js
   ```

## Fuentes
* [Daemon skeleton en c](https://github.com/pasce/daemon-skeleton-linux-c)
* [Npm Dbus next](https://www.npmjs.com/package/dbus-next)
* [Enum state NetworkManager](https://people.freedesktop.org/~lkundrak/nm-docs/nm-dbus-types.html)