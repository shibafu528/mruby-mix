MRuby::Gem::Specification.new('mruby-mix-miquire-fs') do |spec|
  spec.license     = 'MIT'
  spec.authors     = ['shibafu528']
  spec.version     = '1.0.0'
  spec.summary     = %q{Embeddable mikutter runtime - Miquire for file system}

  spec.add_dependency 'mruby-mix'
  spec.add_dependency 'mruby-require', :mgem => 'mruby-require', :checksum_hash => 'b2b95a27b8658c46c5cb05abd168e77a12aef2a4'
  spec.add_dependency 'mruby-dir', :mgem => 'mruby-dir'
  spec.add_dependency 'mruby-yaml', :mgem => 'mruby-yaml'
end
