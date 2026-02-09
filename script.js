// === Definición de patrones ===
const tokenRules = [
  { type: "KEYWORD", regex: /\b(if|else|while|for|return|int|float|string|bool|true|false|void)\b/g, state: "S1" },
  { type: "NUMBER", regex: /\b\d+(\.\d+)?([eE][+-]?\d+)?\b/g, state: "S1" },
  { type: "IDENTIFIER", regex: /\b[a-zA-Z_][a-zA-Z0-9_]*\b/g, state: "S1" },
  { type: "STRING", regex: /"([^"\\]|\\.)*"/g, state: "S3" },
  { type: "OPERATOR", regex: /(\+\+|--|\+=|-=|\*=|\/=|==|!=|<=|>=|&&|\|\||[+\-*/%=<>&|!])/g, state: "S2" },
  { type: "PUNCTUATION", regex: /[(){}\[\],;.]/g, state: "S5" },
  { type: "COMMENT", regex: /(\/\/.*|\/\*[\s\S]*?\*\/)/g, state: "S4" },
];

// === Analizador Léxico ===
function analizar(codigo) {
  const lineas = codigo.split("\n");
  const tokens = [];
  let id = 1;

  lineas.forEach((linea, i) => {
    tokenRules.forEach(rule => {
      let match;
      while ((match = rule.regex.exec(linea)) !== null) {
        tokens.push({
          id: id++,
          type: rule.type,
          lexeme: match[0],
          line: i + 1,
          column: match.index + 1,
          state: rule.state
        });
      }
    });
  });
  return tokens.sort((a,b) => (a.line - b.line) || (a.column - b.column));
}

async function simularAutomata(tokens) {
  const states = document.querySelectorAll(".state");
  for (let s of states) s.classList.remove("active");

  for (let i = 0; i < tokens.length; i++) {
    const token = tokens[i];
    const active = document.getElementById(token.state);
    if (!active) continue;

    active.classList.add("active");
    await new Promise(r => setTimeout(r, 800)); 
    active.classList.remove("active");
  }
}

function mostrarTokens(tokens) {
  const tbody = document.querySelector("#result tbody");
  tbody.innerHTML = "";
  tokens.forEach(t => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td>${t.id}</td>
      <td>${t.type}</td>
      <td>${t.lexeme}</td>
      <td>${t.line}</td>
      <td>${t.column}</td>`;
    tbody.appendChild(row);
  });
}

document.getElementById("analyze").addEventListener("click", async () => {
  const code = document.getElementById("code").value;
  const tokens = analizar(code);
  mostrarTokens(tokens);
  await simularAutomata(tokens);
});

document.getElementById("clear").addEventListener("click", () => {
  document.getElementById("code").value = "";
  document.querySelector("#result tbody").innerHTML = "";
});
