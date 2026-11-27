classdef Library
    %LIBRARY Access to the C API of the VERNIER library (internal).
    %
    %   The bindings call the shared library of the C API (vernier_c) through
    %   loadlibrary, so they need no compilation of their own. This class finds
    %   and loads it, and turns the status codes it returns into errors with
    %   the identifier 'vernier:error'.
    %
    %   The library and its header are looked up next to the +vernier package.
    %   To use another build, load it yourself before the first call:
    %
    %       vernier.Library.load('/path/to/libvernier_c.so', '/path/to/vernier.h')

    % This file is part of the VERNIER Library.
    %
    % Copyright (c) 2018-2026 CNRS, ENSMM, UMLP.

    properties (Constant)
        alias = 'vernier_c'   % name the library is loaded under
    end

    methods (Static)

        function load(libraryPath, headerPath)
            %LOAD Loads the C API library, unless it is already loaded.
            if libisloaded(vernier.Library.alias)
                return;
            end
            folder = fileparts(fileparts(mfilename('fullpath')));
            if nargin < 1 || isempty(libraryPath)
                libraryPath = fullfile(folder, vernier.Library.filename());
            end
            if nargin < 2 || isempty(headerPath)
                headerPath = fullfile(folder, 'vernier.h');
            end
            if exist(libraryPath, 'file') ~= 2
                error('vernier:error', ['The C API library was not found (%s). Build it with ' ...
                    '-DBUILD_C_API=ON and add its directory to the path.'], libraryPath);
            end
            if exist(headerPath, 'file') ~= 2
                error('vernier:error', 'The C API header was not found (%s).', headerPath);
            end
            % the detectors are opaque structures, which loadlibrary warns about
            state = warning('off', 'MATLAB:loadlibrary:TypeNotFound');
            cleaner = onCleanup(@() warning(state));
            loadlibrary(libraryPath, headerPath, 'alias', vernier.Library.alias);
        end

        function unload()
            %UNLOAD Unloads the C API library. Delete the detectors first.
            if libisloaded(vernier.Library.alias)
                unloadlibrary(vernier.Library.alias);
            end
        end

        function check(status)
            %CHECK Raises the last error of the library if a call failed.
            if ischar(status) || isstring(status)
                ok = strcmp(status, 'VERNIER_OK');
            else
                ok = (status == 0);
            end
            if ~ok
                error('vernier:error', '%s', vernier.Library.lastError());
            end
        end

        function message = lastError()
            %LASTERROR Description of the last failure reported by the library.
            message = calllib(vernier.Library.alias, 'vernierLastError');
            if isempty(message)
                message = 'the VERNIER library reported an error.';
            end
        end

        function version = version()
            %VERSION Version of the C++ library behind the bindings.
            vernier.Library.load();
            version = calllib(vernier.Library.alias, 'vernierVersion');
        end

    end

    methods (Static, Hidden)

        function name = filename()
            % Name of the shared library on this platform.
            if ispc
                name = 'vernier_c.dll';
            elseif ismac
                name = 'libvernier_c.dylib';
            else
                name = 'libvernier_c.so';
            end
        end

        function text = toString(buffer)
            % The text held by a buffer filled by the library, without the
            % padding that follows the terminating null character.
            text = buffer;
            if ~ischar(text)
                text = char(text);
            end
            stop = find(text == char(0), 1);
            if ~isempty(stop)
                text = text(1:stop - 1);
            end
        end

        function value = toBackend(backend)
            % The C enumerator for a backend given as 'CPU' or 'CUDA'.
            if isnumeric(backend)
                value = int32(backend);
                return;
            end
            switch upper(char(backend))
                case 'CPU'
                    value = int32(0);
                case 'CUDA'
                    value = int32(1);
                otherwise
                    error('vernier:error', 'backend must be ''CPU'' or ''CUDA''.');
            end
        end

        function name = fromBackend(value)
            % 'CPU' or 'CUDA' from what the library returned for a backend.
            if ischar(value) || isstring(value)
                name = char(value);
                name = strrep(name, 'VERNIER_BACKEND_', '');
            elseif value == 1
                name = 'CUDA';
            else
                name = 'CPU';
            end
        end

    end

end
