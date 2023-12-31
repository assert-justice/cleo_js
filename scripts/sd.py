import sd_utils

def build(cli: sd_utils.Cli):
    target = 'win'
    if cli.has_args():
        target = cli.match(['win', 'linux'])
    cli.data['target'] = target
    build_dir = f'build_{target}'
    exe_name = 'cleo'
    if target == 'win':
        exe_name += '.exe'
    cli.data['exe_name'] = exe_name
    if not cli.is_dir(build_dir):
        cli.mkdir(build_dir)
        cli.cd(build_dir)
        cli.run(['cmake', '..', '-G', 'Ninja'])
    else:
        cli.cd(build_dir)
    cli.run(['ninja'])
    cli.copy(f'./{exe_name}', '../bin')

def run(cli: sd_utils.Cli):
    build(cli)
    cli.reset()
    cli.cd('bin')
    command = cli.data['exe_name']
    if cli.data['target'] == 'linux':
        command = './' + command
    cli.run([command])

def main():
    with sd_utils.Cli() as cli:
        print(cli.args)
        verb = cli.match(['build','run','add_target'])
        if verb == 'build':
            build(cli)
        elif verb == 'run':
            run(cli)
            

if __name__ == '__main__':
    main()