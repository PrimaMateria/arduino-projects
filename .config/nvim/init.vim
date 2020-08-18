call plug#begin('/home/primamateria/development/projects/arduino-projects/.config/nvim/plugged')

  "Plug 'scrooloose/nerdtree'
  Plug 'ms-jpq/chadtree', {'branch': 'chad', 'do': ':UpdateRemotePlugins'}
  Plug 'scrooloose/nerdcommenter'
  Plug 'sudar/vim-arduino-syntax'
  Plug '907th/vim-auto-save'
  Plug 'christoomey/vim-tmux-navigator'
  Plug 'neoclide/coc.nvim', {'branch': 'release'}
  Plug 'itchyny/lightline.vim'
  Plug 'junegunn/fzf.vim'
  Plug 'smallwat3r/vim-simplicity'
  Plug 'kshenoy/vim-signature'
call plug#end()

filetype plugin indent on
set tabstop=2
set shiftwidth=2
set scrolloff=999
set expandtab
set number

" Color scheme
colorscheme desert
hi Cursor cterm=NONE ctermfg=black ctermbg=yellow guifg=black guibg=yellow
hi Search ctermfg=black ctermbg=grey
hi Normal ctermbg=None

nmap <leader><Space> :b#<CR>
"map <C-n> :NERDTreeToggle<CR>
let g:auto_save = 0
let g:java_highlight_functions = 1

let g:lightline = {
  \ 'colorscheme': 'seoul256',
  \ 'active': {
  \   'left': [ [ 'mode', 'paste' ],
  \             [ 'readonly', 'filename', 'modified' ] ]
  \ },
  \ }

" Fzf
nnoremap <leader><leader> :GFiles<CR>
nnoremap <leader>fi       :Files<CR>
nnoremap <leader>C        :Colors<CR>
nnoremap <leader><CR>     :Buffers<CR>
nnoremap <leader>fl       :Lines<CR>
nnoremap <leader>ag       :Ag! <C-R><C-W><CR>
nnoremap <leader>m        :History<CR>

" Chadtree
nnoremap <C-n> <cmd>CHADopen<cr>

set exrc
set secure
