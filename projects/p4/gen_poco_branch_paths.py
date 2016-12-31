# LICENSE
#
#   This software is dual-licensed to the public domain and under the following
#   license: you are granted a perpetual, irrevocable license to copy, modify,
#   publish, and distribute this file as you see fit.
#
# AUTHOR
#   Forrest Smith

import os

def main():
    output = "poco_branch_paths.txt"
    output_file = open(output, 'w')

    depot_name = "empire"
    depot_root = os.path.abspath('..\..\..')
    poco_root = os.path.join(depot_root, 'thirdparty/poco')
    target_root = os.path.join(depot_root, 'fts_console/code/thirdparty/Poco')

    branch_sources = [
        { 'source': 'Foundation/src',               'target': '' },
        { 'source': 'Foundation/include/Poco/',     'target': '' },
        { 'source': 'Net/src',                      'target': 'Net' },
        { 'source': 'Net/include/Poco/Net',         'target': 'Net' },
    ]

    print 'Depot Root: {0}'.format(depot_root)
    print 'Poco Root: {0}'.format(poco_root)

    for info in branch_sources:
        source = info['source']
        source_path = os.path.join(poco_root, source)

        target = info['target']
        target_path = os.path.join(target_root, target)

        for root, dirs, files in os.walk(source_path):
            for name in files:
                file_absolute_path = os.path.join(root, name)
                file_relative_to_depot_root = os.path.relpath(file_absolute_path, depot_root)

                branch_source = '//{0}/main/{1}'.format(depot_name, file_relative_to_depot_root)
                branch_source = branch_source.replace('\\', '/')

                file_relative_to_source_path = os.path.relpath(file_absolute_path, source_path)
                branch_target_absolute = os.path.join(target_root, target_path, file_relative_to_source_path)
                branch_target = os.path.relpath(branch_target_absolute, depot_root)
                branch_target = '//{0}/main/{1}'.format(depot_name, branch_target)
                branch_target = branch_target.replace('\\', '/')

                output_file.write('{0} {1}\n'.format(branch_source, branch_target))

    output_file.close()
    print 'Paths written to {0}'.format(output)


if __name__ == '__main__':
    main()