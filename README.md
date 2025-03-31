# 📌 **Descrição do Funcionamento**

Este sistema foi projetado para **controlar a passagem de pedestres** em um local onde também operam **empilhadeiras** e **pontes rolantes**, utilizando **LEDs** e um **display OLED** para fornecer informações visuais.

🎯 **Objetivo Principal:** Garantir que a passagem de pedestres seja controlada enquanto a empilhadeira está operando, evitando interrupções frequentes no trabalho dos operadores.

---

## 🚦 **Indicações Visuais**

O sistema utiliza três **LEDs** para sinalizar o status da passagem:

- 🔴 **LED Vermelho** → Passagem **não autorizada** 🚫
- 🟢 **LED Verde** → Passagem **liberada** ✅
- 🔵 **LED Azul** → Empilhadeira em operação, passagem **bloqueada** ⚠️

📺 **Display OLED (128x64 px)** → Exibe mensagens e um **contador de tempo**, facilitando a visualização do estado atual do sistema.

---

## 🎛 **Funcionamento do Sistema**

O controle é feito por **dois botões**:

🔘 **Botão A (Pedestres)**
- Quando pressionado, indica a presença de pedestres.
- Ativa uma **contagem regressiva de 5 segundos** ⏳.
- Durante a contagem, o **LED Vermelho** 🔴 acende, bloqueando a passagem.

🔘 **Botão B (Empilhadeira)**
- Indica que a empilhadeira está em operação 🚜.
- O sistema exibe **"AGUARDE A EMPILHADEIRA"** no display.
- O **LED Azul** 🔵 acende, bloqueando a passagem dos pedestres **até o fim da operação**.

🟢 Quando **não há pedestres**, o **LED Verde** acende e o display exibe **"SEM PEDESTRES"**, permitindo a passagem livre.

---

## ⚖ **Hierarquia de Prioridades**

1️⃣ **Empilhadeira (Botão B) tem prioridade máxima** → Bloqueia qualquer outra interação. Os pedestres devem aguardar até a operação ser concluída.

2️⃣ **Pedestres (Botão A) têm prioridade secundária** → O fluxo é gerenciado por uma contagem regressiva de **5 segundos** antes da liberação da passagem.

✅ **Se não houver pedestres ou empilhadeiras em operação, a passagem é liberada imediatamente!**

---

🚀 **Este sistema melhora a segurança e eficiência no ambiente de trabalho, garantindo um fluxo organizado entre pedestres e veículos industriais!**



