function build_mex()
% build_mex  Compile buck-boost control S-function for simulation.
%
% Usage:
%   >> cd(simulation/simulink); build_mex;

    root = fileparts(mfilename('fullpath'));
    repo = fullfile(root, '..', '..');
    fw_app = fullfile(repo, 'firmware', 'app');
    fw_drivers = fullfile(repo, 'firmware', 'drivers');
    fw_bsp = fullfile(repo, 'firmware', 'bsp');

    incPaths = {fw_app, fw_drivers, fw_bsp, root};
    src = { ...
        fullfile(root, 'buckboost_sfunc.c'), ...
        fullfile(root, 'hal_sim_stub.c'), ...
        fullfile(fw_app, 'buckboost_ctrl.c'), ...
        fullfile(fw_app, 'buckboost_fault.c'), ...
        fullfile(fw_app, 'buckboost_state_machine.c'), ...
        fullfile(fw_drivers, 'buckboost_hal.c') ...
    };

    incArgs = strcat('-I', incPaths);
    mex('-R2018a', '-largeArrayDims', 'CFLAGS=$CFLAGS -std=c99', ...
        '-output', 'buckboost_sfunc', incArgs{:}, src{:});
end

/************************ (C) COPYRIGHT 2024 *****END OF FILE****/
