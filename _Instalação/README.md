# 🛠️ Instalação do Arduino IDE e Configuração do ESP32

## 📥 Passo 1: Instalar o Arduino IDE

1. **Baixe o Arduino IDE**:
   - Acesse o [⬇️ site oficial do Arduino](https://www.arduino.cc/en/software).
   - Escolha a versão adequada para o seu sistema operacional (Windows, macOS, Linux) e faça o download.

2. **Instale o Arduino IDE**:
   - **Windows**: Execute o arquivo `.exe` baixado e siga as instruções na tela.
   - **macOS**: Abra o arquivo `.dmg` e arraste o Arduino IDE para a pasta **Aplicativos**.
   - **Linux**: Descompacte o arquivo baixado e execute o script `install.sh` no terminal.

---

## 🔧 Passo 2: Adicionar a Placa ESP32

1. **Abra o Arduino IDE**.
2. **Acesse as Preferências**:
   - No menu, clique em **Arquivo** > **Preferências** (ou use o atalho `Ctrl + ,`).

3. **Adicionar a URL da Placa ESP32**:
   - Na janela de preferências, localize o campo **URLs adicionais para gerenciadores de placas**.
   - Adicione a seguinte URL:
     ```plaintext
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Se já houver URLs lá, separe-as com uma vírgula.

4. **Abrir o Gerenciador de Placas**:
   - Vá para o menu **Ferramentas** > **Placa** > **Gerenciador de Placas**.

5. **Instalar a Placa ESP32**:
   - No campo de pesquisa, digite `ESP32`.
   - Quando a lista aparecer, encontre a opção **esp32 by Espressif Systems** e clique em **Instalar**.

6. **Selecionar a Placa ESP32**:
   - Após a instalação, vá para **Ferramentas** > **Placa** e escolha a placa ESP32 que você está usando (por exemplo, **ESP32 Dev Kit V1**).

---

## 💡 Acendendo o LED da Placa ESP32

Agora que o Arduino IDE está configurado, vamos testar a placa ESP32 acendendo o LED embutido.

### 📝 Passo 1: Escreva o Código

No Arduino IDE, copie e cole o seguinte código:

```cpp
// Definindo o pino do LED embutido
const int ledPin = 2;  // O LED está conectado ao pino GPIO 2 no ESP32

void setup() {
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Liga o LED
  digitalWrite(ledPin, HIGH);
  delay(1000);  // Espera 1 segundo

  // Desliga o LED
  digitalWrite(ledPin, LOW);
  delay(1000);  // Espera 1 segundo
}
```

### 🔌 Passo 2: Carregar o Código para o ESP32

1. Conecte a placa ESP32 ao computador usando um cabo USB.
2. No Arduino IDE, selecione a placa correta:
   - Vá para **Ferramentas** > **Placa** e escolha `DOIT ESP32 DEVKIT V1`.
   - Em **Ferramentas** > **Porta**, selecione a porta à qual o ESP32 está conectado.

3. Clique no botão de **Upload** (ícone de seta para a direita) no Arduino IDE.

4. **Pressione o botão "BOOT"** na sua placa ESP32 assim que o processo de upload começar. Mantenha o botão pressionado até que o upload seja concluído.

### ✅ Passo 3: Verificar o Funcionamento

Após o código ser carregado com sucesso, o LED embutido na placa ESP32 deve começar a piscar com intervalos de 1 segundo.

---

## ❌ Possíveis Erros

### O Windows Não Reconhece a Placa ESP32

Se o Windows não reconhecer a sua placa ESP32, é possível que seja um problema de driver para comunicação USB, e pode ser necessário instalar o driver correspondente.

#### Solução:

1. Baixe e instale os drivers CP210x (versão v6.7.6):
   - Baixe o arquivo da Silicon Labs: [⬇️ CP210x Windows Drivers v6.7.6](./Drivers/CP210x_VCP_Windows.zip).
   - Escolha a versão correta para o seu sistema operacional (32-bit ou 64-bit).
   - Siga as instruções de instalação fornecidas.

2. Após a instalação, desconecte e reconecte a placa ESP32 ao computador.
3. Verifique se a porta é reconhecida no Thonny IDE em **Tools** > **Options** > **Interpreter** na seção **Port**.

Se o problema persistir, tente reiniciar o computador e verificar novamente as conexões da placa.

