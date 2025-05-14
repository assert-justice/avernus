import sd_utils
import platform

def main():
    exe_ext = '.exe' if platform.system() == 'Windows' else ''
    with sd_utils.Cli() as cli:
        if not cli.is_dir('build'):
            cli.mkdir('build')
        cli.cd('build')
        if not cli.is_file('build.ninja'):
            cli.run('cmake .. -G Ninja'.split(' '))
        cli.run(['ninja'])
        cli.reset()
        if not cli.is_dir('dist'):
            cli.mkdir('dist')
        cli.copy(f'./build/avernus{exe_ext}', './dist')
        cli.cd('dist')
        cli.run([f'./avernus{exe_ext}'])

if __name__ == "__main__":
    main()