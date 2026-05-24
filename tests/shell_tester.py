# Desenvolvido por L. A. Leandro - Sao Jose dos Campos - SP - 24/05/2026
"""
Testes de Integracao para o Mini-Shell
Executa o binario e simula entrada de comandos via stdin,
validando saidas esperadas e codigos de retorno.
"""

import subprocess
import sys
import os
import tempfile


BINARY = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "minishell")


def run_test(name, commands, expected_exit_code=0, expected_output_fragments=None):
    """
    Executa uma bateria de comandos no Mini-Shell e verifica o resultado.
    """
    print(f"[TESTE] {name}... ", end="", flush=True)

    try:
        proc = subprocess.run(
            [BINARY],
            input="\n".join(commands),
            capture_output=True,
            text=True,
            timeout=5,
        )
    except FileNotFoundError:
        print("FALHA - binario nao encontrado. Compile com 'make' primeiro.")
        return False
    except subprocess.TimeoutExpired:
        print("FALHA - tempo limite excedido (5s)")
        return False

    if proc.returncode != expected_exit_code:
        print(f"FALHA - codigo de retorno esperado {expected_exit_code}, obteve {proc.returncode}")
        return False

    if expected_output_fragments:
        for fragment in expected_output_fragments:
            if fragment not in proc.stdout and fragment not in proc.stderr:
                print(f"FALHA - fragmento esperado nao encontrado: '{fragment}'")
                return False

    print("OK")
    return True


def test_exit():
    """Testa o comando embutido exit."""
    return run_test("Exit", ["exit"], expected_exit_code=0)


def test_empty_input():
    """Testa entrada vazia (apenas Enter)."""
    return run_test("Entrada vazia", ["", "exit"], expected_exit_code=0)


def test_echo():
    """Testa execucao de comando externo simples."""
    return run_test("echo", ["echo ola mundo", "exit"],
                    expected_output_fragments=["ola mundo"])


def test_ls():
    """Testa execucao de ls."""
    return run_test("ls", ["ls", "exit"],
                    expected_output_fragments=["Makefile", "main.cpp"])


def test_pwd():
    """Testa pwd."""
    return run_test("pwd", ["pwd", "exit"],
                    expected_output_fragments=["Mini-Shell"])


def test_cd_pwd():
    """Testa cd seguido de pwd para verificar mudanca de diretorio."""
    return run_test("cd e pwd", ["cd ..", "pwd", "exit"])


def test_cd_sem_argumento():
    """Testa cd sem argumento (deve ir para HOME)."""
    return run_test("cd sem argumento", ["cd", "pwd", "exit"])


def test_comando_invalido():
    """Testa comando inexistente - nao deve crashar."""
    return run_test("Comando inexistente", ["comando_que_nao_existe_123", "exit"],
                    expected_exit_code=0)


def test_comandos_multiplos():
    """Testa multiplos comandos em sequencia."""
    commands = [
        "echo primeiro",
        "echo segundo",
        "echo terceiro",
        "exit",
    ]
    return run_test("Comandos multiplos", commands,
                    expected_output_fragments=["primeiro", "segundo", "terceiro"])


def test_whitespace_extra():
    """Testa entradas com espacos extras."""
    return run_test("Espacos extras", ['   echo    espacos    ', "exit"],
                    expected_output_fragments=["espacos"])


def main():
    tests = [
        test_exit,
        test_empty_input,
        test_echo,
        test_ls,
        test_pwd,
        test_cd_pwd,
        test_cd_sem_argumento,
        test_comando_invalido,
        test_comandos_multiplos,
        test_whitespace_extra,
    ]

    passed = 0
    failed = 0

    for test in tests:
        if test():
            passed += 1
        else:
            failed += 1

    total = passed + failed
    print(f"\n{'='*40}")
    print(f"RESUMO: {passed}/{total} testes passaram")
    print(f"{'='*40}")

    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
