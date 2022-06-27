$(document).ready(function() {
    var score = 0;
    var quiz = [
        ['O QUE É ARDUINO?', 'ARDUINO é uma plataforma flexível open-source de hardware e software para prototipagem eletrônica.', ['ARDUINO é um sistema operacional de software para eletrônicos.', 'ARDUINO é uma plataforma rígida open-source de hardware para prototipagem eletrotônica.', 'ARDUINO é uma plataforma flexível open-source de hardware e software para prototipagem eletrônica.', 'ARDUINO é uma plataforma de prototipagem mecânica.']],
        ['Quantas e como são classificadas os pinos da placa Arduino Uno?', '14 pinos de entra e saída digital (pinos 0-13): Esses pinos podem ser utilizados como entradas ou saídas digitais de acordo com a necessidade do projeto e conforme foi definido no sketch criado na IDE.', ['10 pinos de entra e saída digital (pinos 2-9): Esses pinos podem ser utilizados como entradas ou saídas digitais de acordo com a necessidade do projeto e conforme foi definido no sketch criado na IDE.', '14 pinos de entra e saída digital (pinos 0-13): Esses pinos podem ser utilizados como entradas ou saídas digitais de acordo com a necessidade do projeto e conforme foi definido no sketch criado na IDE.', '16 pinos de entra e saída digital (pinos 4-14): Esses pinos podem ser utilizados como entradas ou saídas digitais de acordo com a necessidade do projeto e conforme foi definido no sketch criado na IDE.', '12 pinos de entra e saída digital (pinos 0-11): Esses pinos podem ser utilizados como entradas ou saídas digitais de acordo com a necessidade do projeto e conforme foi definido no sketch criado na IDE.']],
        ['Qual a linguagem usada pelo Arduino?', 'Linguagem C', ['Linguagem C', 'Java', 'Python', 'GO']],
        ['Qual a fonção de um resistor?', 'Resistores são componentes eletrônicos cuja principal função é limitar o fluxo de cargas elétricas por meio da conversão da energia elétrica em energia térmica.', ['Resistores são componentes eletrônicos cuja principal função é limitar o fluxo de cargas elétricas por meio da conversão da energia elétrica em energia térmica.', 'Filtrar a energia potencializando-a.', 'Os resistor tem a função de ser uma ponte aletrica para componentes.', 'Enfeitar os componentes eletricos.']],
        ['Qual direção os motores podem girar?', 'Para esquerda e direita', ['Para esquerda.', 'Para direita', 'Para esquerda e direita', 'Para a direção ao qual o motor está programado para girar.']]
      ];
    var totalQuestoes = quiz.length;
  
    $('#next').click(function() {
        $(this).hide();
        $('#result').text('');
        if (quiz.length == totalQuestoes) {
          $(this).val('Próxima pergunta');
        } else if (quiz.length == 1) {
          $(this).val('Mostrar resultados');
        }
        if (quiz.length > 0) {
          pergunta();
        } else {
          mostrarResultados();
        }
    });


    //função que cria as perguntas
    function pergunta() {
      // pega uma questão
      var currentQ = quiz.pop();
      // pega a pergunta
      $('#question').text(currentQ[0]);
      //chama a função que cria as opções
      var answers = criaRespostas(currentQ[2]);
      // add choices to the page
      $('#answers').html(answers);
      // verifica a resposta no clique
      $('#answers input').click(function() {
        $('#answers input').attr("disabled",true);
        if ($(this).val() == currentQ[1]) {
          score++;
          $('#result').prepend('Correto!');
        } else {
          $('#result').prepend('Incorreto. A resposta certa é: ' + currentQ[1]);
        }
        // mostra o botão de proxima pergunta
        $('#next').show();
      });
    }
  
    //função que cria as respostas
    function criaRespostas(answers) {
      var answerHTML = '';
      // loop de cada opção
      for (i = 0; i < answers.length; i++) {
        //constroi as opções
        answerHTML += '<div class="inputGroup"> <input name="radio" type="radio" id="radio'+i+'" value="';
        answerHTML += answers[i] + '"> ' + '<label for="radio'+i+'">'+answers[i]+'</label>' + '</div>';
      }
      //retorna o HTML completo com todas as opções
      return answerHTML;
    }
  
    //função que mostra os resultados
    function mostrarResultados() {
      var results = '<p>Você acertou ' + score + ' de ' + totalQuestoes + ' questões corretamente.</p><br><button onClick="history.go(0);" class="quiz-btn">Refazer Quiz</button>';
      $('#questionArea').html(results);
    }
  
    
  
  
  }); // end ready
  