import subprocess
import sd_utils
import os

# subprocess.run(['run.bat'])

def build(cli: sd_utils.Cli):
    if not cli.is_dir('build'):
        cli.mkdir('build')
        cli.cd('build')
        cli.run(['cmake', '..', '-G', 'Ninja'])
    else:
        cli.cd('build')
    cli.run(['ninja'])
    cli.copy('./cleo.exe', '../bin')

def main():
    with sd_utils.Cli() as cli:
        verb = cli.match(['build','run','add_target'])
        if verb == 'build':
            build(cli)
        elif verb == 'run':
            build(cli)
            cli.reset()
            cli.cd('bin')
            cli.run('cleo.exe')

if __name__ == '__main__':
    main()